#pragma once

#include <cstdint>
#include <functional>

namespace net {

constexpr int SESSION_INVALID_ID = 0;

struct session_id {
    uint64_t value;

    constexpr explicit session_id(uint64_t v) noexcept : value(v) {}
    constexpr bool operator==(const session_id& other) const noexcept { return value == other.value; }
    constexpr bool is_valid() const noexcept { return value != SESSION_INVALID_ID; }
    constexpr uint64_t get() const noexcept { return value; }
};

struct session_id_hash { // map container compatibility
    size_t operator()(const session_id& id) const noexcept { return std::hash<uint64_t>{}(id.value); }
};
} // namespace net