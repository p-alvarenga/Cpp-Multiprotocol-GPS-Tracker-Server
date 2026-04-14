#include "net/session/session.h"

#include <cstdint>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#include "core/log.h"
#include "net/constants.h"
#include "protocol/protocol.h"

[[__nodiscard__("net::session::start() return must not be discarded")]]
bool net::session::session::start() noexcept {
    if (socket_fd < 0) return false;
    if (!id.is_valid()) return false;

    core::log::info("starting session id=%d fd=%d", id.get(), socket_fd);

    if (!is_running()) {
        th = std::thread(&session::loop, this);
        running.store(true);
    }

    return true;
}

void net::session::session::loop() noexcept {
    while (is_running()) {
        memset(read_buffer, 0, constants::session_read_buffer_size);

        ssize_t n_recv = ::read(socket_fd, read_buffer, constants::session_read_buffer_size);
        if (n_recv == 0) {
            core::log::warn("session (id=%d fd=%d) disconnected. closing...", id.get(), socket_fd);
            request_stop();
            return;
        }

        if (n_recv < 0) {
            core::log::err("n_recv = %d", n_recv);
            if (errno == EINTR) continue;
            return;
        }

        if (protocol == protocol::type::unknown) {
            protocol =
                protocol::identify_protocol(reinterpret_cast<const uint8_t*>(read_buffer), static_cast<size_t>(n_recv));

            if (protocol == protocol::type::unknown) {
                core::log::warn("could not identify protocol (session %d)", id.get());
                continue;
            }

            protocol_desc = protocol::registry::resolve(protocol);
            if (!protocol_desc) {
                protocol = protocol::type::unknown;
                continue;
            }

            framer = protocol_desc->make_framer();
        }

        if (protocol != protocol::type::unknown && framer) {
            framer->feed(reinterpret_cast<const uint8_t*>(read_buffer), static_cast<size_t>(n_recv));

            protocol::frame f;
            protocol::packet pkt;

            while (framer->next(f)) {
                if (!protocol_desc->decoder->decode(f, pkt)) {
                    core::log::err("session[%d]=could not decode packet", id.get());
                    continue;
                }

                emit(pkt.msg);
            }
        }
    }
}

void net::session::session::request_stop() noexcept {
    if (running.exchange(false)) return;

    ::shutdown(socket_fd, SHUT_RDWR);
    ::close(socket_fd);
}

void net::session::session::stop() noexcept {
    request_stop();

    if (std::this_thread::get_id() == th.get_id()) [[__unlikely__]] {
        core::log::warn("thread %d (session %d) tried to self join (deadlock)", th.get_id(), id.get());
        return;
    }

    if (th.joinable()) th.join();
}