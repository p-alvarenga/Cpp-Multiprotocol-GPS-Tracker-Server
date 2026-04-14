#pragma once

#include <thread>

#include "evt/router/event_queue.h"
#include "evt/session/constants.h"
#include "evt/session/session_event.h"
#include "net/constants.h"
#include "net/session_id.h"
#include "protocol/protocol.h"

namespace net::session {
class session {
private:
    protocol::type protocol{protocol::type::unknown};
    const protocol::protocol_descriptor* proto_desc{nullptr}; // decoder & encoder
    std::unique_ptr<protocol::i_framer> framer{nullptr};

    int socket_fd{-1};
    net::session_id id;

    std::thread th;
    std::atomic<bool> running{false};

    char read_buffer[constants::session_read_buffer_size];
    evt::event_queue<evt::session::event>& sink;

    core::imei* device_imei{nullptr};
    protocol::packet* last_packet{nullptr};

    void emit(const msg::message& m) {
        evt::session::event ev(id, device_imei);
        ev.set_message(m);
        sink.push(ev);
    }

    void emit(const evt::session::error_code e) {
        evt::session::event ev(id, device_imei);
        ev.set_error(e);
        sink.push(ev);
    }

public:
    bool start() noexcept; // start = init() + run()
    void loop() noexcept;

    void request_stop() noexcept;
    void stop() noexcept;

    bool is_running() const noexcept { return running.load(); }
    session_id get_id() const noexcept { return id; }

    session(int fd, const net::session_id& id, evt::event_queue<evt::session::event>& sink) noexcept
        : socket_fd(fd), id(id), sink(sink) {}

    ~session() noexcept { stop(); }
};

} // namespace net::session