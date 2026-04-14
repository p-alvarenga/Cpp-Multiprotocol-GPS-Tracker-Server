#include "net/manager/session_manager.h"

#include "core/log.h"

[[__nodiscard__("net::session_manager::create_session return must not be discarded")]]
bool net::session::manager::create_session(int fd) noexcept {
    assert(sink);

    { // verify if already exists
        std::lock_guard lock(m);
        if (fd_index.contains(fd)) {
            core::log::err("session_manager: session with fd %d already exists", fd);
            return false;
        }
    }
    // create and start session
    session_id id = next_id();
    std::unique_ptr<session> s = std::make_unique<session>(fd, id, *sink);

    if (!s->start()) {
        core::log::err("could not start session with id=%d fd=%d", id.get(), fd);
        return false;
    }

    { // lock for adding into hashed map
        std::lock_guard lock(m);

        sessions.emplace(id, std::move(s));
        fd_index.emplace(fd, id);
    }

    core::log::info("successfully created session id=%d with fd=%d", id.get(), fd);
    return true;
}

void net::session::manager::shutdown() noexcept {
    std::lock_guard<std::mutex> lk(m);
    for (auto it = sessions.begin(); it != sessions.end();) {
        it->second->stop();
        it = sessions.erase(it);
    }
}