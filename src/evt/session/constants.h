#pragma once

#include <cstdint>

namespace evt::session {
enum class kind : uint8_t {
    none,

    connected = 0,
    disconnected,

    message,
    error,

    sys_shutdown,
};

enum class error_code : int {
    closed = 0,
    invalid_packet,
};

inline constexpr const char* kind_to_str(kind k) {
    switch (k) {
    case kind::connected:
        return "SESSION_CONNECTED";
    case kind::disconnected:
        return "SESSION_DISCONNECTED";
    case kind::message:
        return "SESSION_MESSAGE";
    case kind::error:
        return "SESSION_ERROR";
    case kind::sys_shutdown:
        return "SYSTEM_SHUTDOWN";
    }

    __builtin_unreachable();
}

inline constexpr const char* error_code_to_str(error_code err) {
    switch (err) {
    case error_code::closed:
        return "SESSION_CLOSED";
    case error_code::invalid_packet:
        return "SESSION_INVALID_PACKET";
    default:
        return "SESSION_EVENT_UNKNOWN";
    };

    __builtin_unreachable();
}

} // namespace evt::session