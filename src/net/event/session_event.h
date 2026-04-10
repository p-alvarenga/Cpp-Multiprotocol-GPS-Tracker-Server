#pragma once

#include <cassert>
#include <new>
#include <type_traits>

#include "msg/message.h"
#include "net/event/event_def.h"

namespace net {

struct session_event {

private:
    event_kind type;
    union {
        msg::message msg;
        event_error_code err_code;
    };

    session_event() = default;

public:
    static session_event make_message(const msg::message& m) noexcept;
    static session_event make_error(event_error_code err_c) noexcept;

    bool is_message() const noexcept { return type == event_kind::message; }
    bool is_error() const noexcept { return type == event_kind::error; }
    bool operator==(const session_event& other) const noexcept;

    const msg::message& get_message() const noexcept;
    msg::message& get_message() noexcept;
    event_error_code get_error() const noexcept;
};

inline session_event session_event::make_message(const msg::message& m) noexcept {
    session_event ev;
    ev.type = event_kind::message;
    new (&ev.msg) msg::message(m);
    return ev;
}

inline session_event session_event::make_error(event_error_code err_c) noexcept {
    session_event ev;
    ev.type = event_kind::error;
    new (&ev.err_code) event_error_code(err_c);
    return ev;
}

inline bool session_event::operator==(const session_event& other) const noexcept {
    if (type != other.type) return false;

    switch (type) {
    case event_kind::message:
        return msg == other.get_message();

    case event_kind::error:
        return err_code == other.get_error();

    default: // connected || disconnected
        return true;
    }
}

inline const msg::message& session_event::get_message() const noexcept {
    assert(is_message());
    return msg;
}

inline msg::message& session_event::get_message() noexcept {
    assert(is_message());
    return msg;
}

inline event_error_code session_event::get_error() const noexcept {
    assert(is_error());
    return err_code;
}

static_assert(std::is_trivially_copyable_v<net::session_event>);
static_assert(std::is_trivially_copyable_v<net::event_kind>);
static_assert(std::is_trivially_copyable_v<net::event_error_code>);

}; // namespace net