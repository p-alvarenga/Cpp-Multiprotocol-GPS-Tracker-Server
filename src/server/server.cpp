#include "server.h"

#include <cstring>
#include <sys/socket.h>

#include "core/log.h"

[[__nodiscard__]]
bool server::server::init() noexcept {
    if (port < 0 || port > 65535) {
        core::log::err("server: port %d is not valid", port);
        return false;
    }

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        core::log::err("socket file descriptor value (%d) is not valid", socket_fd);
        return false;
    }

    int opt = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0 (need to be configurable)
    addr.sin_port = htons(port);

    if (bind(socket_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        core::log::err("server: bind failed fd=%d errno=%d (%s)", socket_fd, errno, std::strerror(errno));
        return false;
    }

    // managers
    session_manager.bind_sink(router.get_sessions_sink());
    router.bind_session_manager(session_manager);

    return true;
}

[[__nodiscard__]]
bool server::server::run() noexcept {
    if (listen(socket_fd, n_connections) < 0) return false;

    core::log::info("server: started listen at %d", port);
    running.store(true);

    if (!router.start()) {
        core::log::err("server: event router could not start");

        return false;
    }

    while (running.load()) {
        sockaddr_in session_addr{};
        socklen_t session_len = sizeof(session_addr);

        int session_fd = accept(socket_fd, (sockaddr*)&session_addr, &session_len);

        if (session_fd < 0) [[unlikely]] {
            if (errno == EINTR) continue;

            core::log::warn("accept failed: %d (%s)", errno, std::strerror(errno));
            continue;
        }

        session_manager.create_session(session_fd);
    }

    return true;
}

void server::server::shutdown() noexcept {
    running.store(false);

    if (socket_fd >= 0) {
        close(socket_fd);
    }

    router.request_shutdown();
    router.join();

    session_manager.shutdown();
}