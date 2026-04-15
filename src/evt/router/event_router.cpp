#include "evt/router/event_router.h"

#include "core/log.h"
#include "evt/session/constants.h"
#include "evt/session/session_event.h"

[[__nodiscard__("evt::event_router::start() discard must not be discarded")]]
bool evt::router::start() noexcept {
    if (session_manager == nullptr) {
        core::log::err("session manager pointer is nullptr");
        return false;
    }

    if (!running.load()) {
        th = std::thread(&router::run, this);
        running.store(true);
    }

    return true;
}

void evt::router::run() noexcept {
    core::log::info("evt::router started");

    dispatch_session_event(); // thread
}

void evt::router::dispatch_session_event() noexcept {
    while (running.load()) {

        evt::session::event ev = session_sink.wait_and_pop();
        evt::session::kind ev_which = ev.which();

        if (ev_which == evt::session::kind::sys_shutdown) return;

        if (ev_which == evt::session::kind::message) {
            msg::message msg = ev.get_message();

            switch (msg.which()) {
            case msg::kind::login:
                core::log::info("device %s successfully logged into server", msg.get_login()->imei.get());
                return;
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

        if (ev_which == evt::session::kind::error) {
            evt::session::error_code err = ev.get_error();

            core::log::err("event_router: session event has error %s", session::error_code_to_str(err));
        }

        core::log::err("event kind not treated - kind=%d (%s)", ev_which, session::kind_to_str(ev_which));
    }
}

void evt::router::request_shutdown() noexcept {
    running.store(false);
    session_sink.push(evt::session::event::make_shutdown());
}