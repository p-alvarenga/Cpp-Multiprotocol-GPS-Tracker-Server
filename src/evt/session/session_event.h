#pragma once

#include <cassert>
#include <new>

#include "./constants.h"
#include "msg/message.h"
#include "net/session_id.h"

namespace evt::session { //

struct event {

private:
    kind type;

    const net::session_id& id;
    const core::imei* imei;

    union {
        msg::message msg;
        error_code err_code;
    };

public:
    bool is_message() const noexcept { return type == kind::message; }
    bool is_error() const noexcept { return type == kind::error; }

    kind which() const noexcept { return type; };

    bool operator==(const event& other) const noexcept;

    const msg::message& get_message() const noexcept;
    msg::message& get_message() noexcept;
    error_code get_error() const noexcept;
    const net::session_id get_id() const noexcept { return id; }
    const core::imei* get_imei() const noexcept { return imei; };

    void set_message(const msg::message& m) noexcept;
    void set_error(const error_code errc) noexcept;

    event(const net::session_id& id, const core::imei* imei) noexcept : type(kind::none), id(id), imei(imei) {};
};

inline bool event::operator==(const event& other) const noexcept {
    if (type != other.type) return false;

    if (type == kind::message) return msg == other.get_message();
    if (type == kind::error) return err_code == other.get_error();

    return true; // connected || disconnected
}

inline const msg::message& event::get_message() const noexcept {
    assert(type == kind::message);
    return msg;
}

inline error_code event::get_error() const noexcept {
    assert(type == kind::error);
    return err_code;
}

inline void event::set_message(const msg::message& m) noexcept {
    new (&msg) msg::message(m);
    type = kind::message;
}

inline void event::set_error(const error_code errc) noexcept {
    type = kind::error;
    err_code = errc;
}
} // namespace evt::session