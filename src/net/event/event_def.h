#pragma once

namespace net {
enum class event_kind : int {
    connected = 0,
    disconnected,

    message,
    error,
};

enum class event_error_code : int {
    closed = 0,
    invalid_packet,
};

inline const char* event_kind_to_str(event_kind k) {
    switch (k) {
    case event_kind::connected:
        return "SESSION_CONNECTED";
    case event_kind::disconnected:
        return "SESSION_DISCONNECTED";
    case event_kind::message:
        return "SESSION_MESSAGE";
    case event_kind::error:
        return "SESSION_ERROR";

    default:
        return "SESSION_ERORR_UNKNOWN";
    }
}

inline const char* event_error_code_to_str(event_error_code e) {
    switch (e) {
    case event_error_code::closed:
        return "SESSION_CLOSED";
    case event_error_code::invalid_packet:
        return "SESSION_INVALID_PACKET";

    default:
        return "SESSION_EVENT_UNKNOWN";
    };
}

} // namespace net