#include "server.h"

#include <sys/socket.h>

#include "core/log.h"

namespace server {

[[__nodiscard__]]
bool server::init() noexcept {
    if (port < 0) return false;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) return false;

    int opt = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0 (need to be configurable)
    addr.sin_port = htons(port);

    if (bind(socket_fd, (sockaddr*)&addr, sizeof(addr)) < 0) return false;

    // managers
    sessions.bind_sink(router.get_sessions_sink());
    router.bind_session_manager(sessions);

    return true;
}

[[__nodiscard__]]
bool server::run() noexcept {
    if (listen(socket_fd, n_connections) < 0) return false;

    core::log::info("server: started listen at %d", port);
    running.store(true);

    while (running.load()) {
        sockaddr_in session_addr{};
        socklen_t session_len = sizeof(session_addr);

        int session_fd = accept(socket_fd, (sockaddr*)&session_addr, &session_len);
        if (session_fd < 0) [[unlikely]] {
            core::log::warn("socked fd less than 0: %d", session_fd);
            continue;
        }

        sessions.create_session(session_fd);
    }

    return true;
}

} // namespace server