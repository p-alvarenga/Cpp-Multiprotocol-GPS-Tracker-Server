#pragma once
#include <cstdint>
#pragma once

#include <cstdarg>
#include <cstdio>
#include <unistd.h>
#include <utility>

namespace core::log {

constexpr int log_buffer_size = 512;
constexpr int log_hex_buffer_size = 1024;

template <typename... Args> inline void log_impl(const char* level, const char* fmt, Args&&... args) {
    char buf[log_buffer_size];

    int offset = std::snprintf(buf, sizeof(buf), "%s ", level);
    if (offset < 0 || offset >= (int)sizeof(buf)) return;

    int n = std::snprintf(buf + offset, sizeof(buf) - offset, fmt, std::forward<Args>(args)...);

    if (n < 0 || n + 1 + offset >= (int)sizeof(buf)) return;

    buf[offset + n] = '\n';
    buf[offset + n + 1] = '\0';

    write(STDOUT_FILENO, buf, offset + n + 1); // +1 due to \n
}

template <typename... Args> inline void info(const char* fmt, Args&&... args) {
    log_impl("[INFO] ", fmt, std::forward<Args>(args)...);
}
inline void info(const char* fmt) { log_impl("[INFO] ", "%s", fmt); }

template <typename... Args> inline void warn(const char* fmt, Args&&... args) {
    log_impl("[WARN] ", fmt, std::forward<Args>(args)...);
}
inline void warn(const char* fmt) { log_impl("[WARN] ", "%s", fmt); }

template <typename... Args> inline void err(const char* fmt, Args&&... args) {
    log_impl("[ERROR]", fmt, std::forward<Args>(args)...);
}
inline void err(const char* fmt) { log_impl("[ERROR]", "%s", fmt); }

inline void hex(const uint8_t* data, size_t size) {
    if (size == 0) return;

    char buf[log_hex_buffer_size];
    size_t pos = 0;

    pos += std::snprintf(buf + pos, sizeof(buf) - pos, "[HEX]   ");

    for (size_t i = 0; i < size; ++i) {
        int n = std::snprintf(buf + pos, sizeof(buf) - pos, "%02X ", data[i]);
        if (n < 0 || pos + n >= (int)sizeof(buf)) break;
        pos += n;
    }

    if (pos < (int)sizeof(buf) - 1) {
        buf[pos++] = '\n';
    }

    std::printf("%s", buf);
}

} // namespace core::log