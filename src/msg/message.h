#pragma once

#include <new>
#include <type_traits>

#include "msg/login_message.h"

namespace msg {

enum class kind : uint8_t {
    login = 0,
    location,
    heartbeat,

    unknown,
};

struct message {
private:
    kind type;

    union {
        login login_msg;
        // location loc_msg;
        // heartbeat heartbeat_msg;
    };

public:
    bool operator==(const message& msg) const noexcept;
    void set_login(const login& l) noexcept {
        new (&login_msg) login(l);
        type = kind::login;
    }
    const login* get_login() const { return type == kind::login ? &login_msg : nullptr; }

    kind which() const noexcept { return type; }
    message() = default; // dangerours
};

inline bool message::operator==(const message& msg) const noexcept {
    if (type != msg.type) {
        return false;
    }

    switch (type) {
    default:
        return true;
    }

    return true;
}

// assure trivial type for union {} use;
static_assert(std::is_trivial_v<message>);
static_assert(std::is_trivially_copyable_v<message>);
static_assert(std::is_trivially_destructible_v<message>);

} // namespace msg