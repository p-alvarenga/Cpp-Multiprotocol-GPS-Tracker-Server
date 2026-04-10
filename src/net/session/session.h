#pragma once

#include <thread>

#include "net/constants.h"
#include "net/event/session_event.h"
#include "net/session_id.h"
#include "proto/proto.h"
#include "router/event_queue.h"

namespace net {

class session {
private:
    proto::protocol_type protocol{proto::protocol_type::unknown};
    std::unique_ptr<proto::i_framer> framer;
    std::unique_ptr<proto::i_decoder> decoder;

    int socket_fd{-1};
    net::session_id id;

    std::thread th;
    std::atomic<bool> running{false};

    char read_buffer[constants::session_read_buffer_size];
    router::ev_queue<session_event>& sink;

public:
    bool start() noexcept; // start = init() + run()
    void loop() noexcept;

    void request_stop() noexcept;
    void stop() noexcept;

    bool is_running() const noexcept { return running.load(); }
    session_id get_id() const noexcept { return id; }

    session(int fd, const net::session_id& id, router::ev_queue<session_event>& sink) noexcept
        : socket_fd(fd), id(id), sink(sink) {}

    ~session() noexcept { stop(); }
};

} // namespace net