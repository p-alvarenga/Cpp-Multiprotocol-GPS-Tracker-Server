#pragma once

#include <thread>

#include "evt/router/event_queue.h"
#include "evt/session/session_event.h"
#include "net/manager/session_manager.h"

namespace evt {

class router {
private:
    net::session::manager* session_manager{nullptr};
    event_queue<evt::session::event> session_sink;

    std::atomic<bool> running{false};
    std::thread th;

    void run() noexcept;

public:
    bool start() noexcept;
    void request_shutdown() noexcept;

    void join() noexcept {
        if (th.joinable()) th.join();
    }

    void bind_session_manager(net::session::manager& s_mgr) noexcept { session_manager = &s_mgr; }
    // bind_device_manager(...) noexcept { ... }

    event_queue<evt::session::event>& get_sessions_sink() noexcept { return session_sink; }
    // ev_queue<dev::dev_event& ...> get_devices_sink() noexcept { ... }

    void dispatch_session_event() noexcept;
    void dispatch_device_event() noexcept;

    explicit router() noexcept = default;
    ~router() noexcept {
        request_shutdown();

        if (std::this_thread::get_id() != th.get_id() && th.joinable()) {
            th.join();
        }
    }
};

} // namespace evt