#include <csignal>

#include "core/log.h"
#include "server/server.h"

server::server* g_server = nullptr;

void handle_sigint(int) {
    if (g_server) {
        core::log::info("received SIGINT: trying graceful shutdown");
        g_server->shutdown();
        return;
    }

    core::log::err("graceful shutdown failed. calling _exit(1)");
    _exit(1);
}

int main() {
    signal(SIGINT, handle_sigint);

    server::server s(9000, 10);
    g_server = &s;

    if (!s.init()) {
        core::log::err("fatal: could not initialize server");
        return 1;
    }

    s.run();

    return 0;
}