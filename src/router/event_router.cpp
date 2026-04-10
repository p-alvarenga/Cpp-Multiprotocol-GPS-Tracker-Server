#include "event_router.h"

#include "core/log.h"

[[__nodiscard__("Must not discard event_router::run return")]]
bool router::event_router::run() {
    running.store(true);

    while (running.load()) {
        dispatch_session_event();
    }

    return true;
}

void router::event_router::dispatch_session_event() noexcept {
    net::session_event ev = session_sink.wait_and_pop();

    if (ev.is_message()) {
        msg::message msg = ev.get_message();

        switch (msg.which()) {
        case msg::kind::login:
            // device_manager.handle_login(login_message);
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
        net::event_error_code err = ev.get_error();

        core::log::err("event_router: session event has error %s", net::event_error_code_to_str(err));
    }
}