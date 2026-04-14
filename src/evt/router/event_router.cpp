#include "evt/router/event_router.h"

#include "core/log.h"
#include "evt/session/constants.h"

[[__nodiscard__("evt::event_router::start() discard must not be discarded")]]
bool evt::router::start() {
    if (session_manager == nullptr) return false;

    if (!running.load()) {
        th = std::thread(&router::run, this);
        running.store(true);
    }

    return true;
}

void evt::router::run() {
    core::log::info("event router started running...");

    while (running.load()) {
        // thread both dispatchers (session & [future] device) //
        dispatch_session_event();
    }
}

void evt::router::dispatch_session_event() noexcept {
    core::log::info("dispatching evt::session::event");

    evt::session::event ev = session_sink.wait_and_pop();

    if (ev.is_message()) {
        msg::message msg = ev.get_message();

        switch (msg.which()) {
        case msg::kind::login:
            core::log::info("device %s successfully logged into server", msg.get_login()->imei.get());

            break;
        case msg::kind::location:
            // device_manager.handle_location(location_message);
            break;
        case msg::kind::heartbeat:
            // device_manager.handle_heartbeat(heartbeat_message);
            break;

        case msg::kind::unknown:
            return; // log invalid msg
        }
    }

    if (ev.is_error()) {
        evt::session::error_code err = ev.get_error();

        core::log::err("event_router: session event has error %s", session::error_code_to_str(err));
    }
}