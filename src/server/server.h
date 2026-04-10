#pragma once

#include <atomic>
#include <netinet/in.h>

#include "router/event_router.h"

namespace server {

class server {
private:
    std::atomic_bool running{false};

    int port{-1};
    int n_connections{-1};

    int socket_fd{-1};
    sockaddr_in addr{};

    net::session_manager sessions;
    router::event_router router;

public:
    bool init() noexcept;
    bool run() noexcept;

    server(int port, int n_conn) noexcept : port(port), n_connections(n_conn) {}
    ~server() noexcept = default;
};

} // namespace server