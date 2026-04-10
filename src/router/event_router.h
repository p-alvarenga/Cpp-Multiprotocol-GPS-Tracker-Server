#pragma once

#include "net/event/session_event.h"
#include "net/manager/session_manager.h"
#include "router/event_queue.h"

namespace router {

class event_router {
private:
    net::session_manager* sessions{nullptr};
    ev_queue<net::session_event> session_sink;

    std::atomic<bool> running{false};

public:
    explicit event_router() noexcept = default;

    void bind_session_manager(net::session_manager& s_mgr) noexcept { sessions = &s_mgr; }
    // bind_device_manager(...) noexcept { ... }

    ev_queue<net::session_event>& get_sessions_sink() noexcept { return session_sink; }
    // ev_queue<dev::dev_event& ...> get_devices_sink() noexcept { ... }

    void dispatch_session_event() noexcept;
    void dispatch_device_event() noexcept;

    bool run();
};
} // namespace router