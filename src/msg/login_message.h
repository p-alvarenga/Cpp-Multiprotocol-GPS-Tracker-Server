#pragma once

#include <cstdint>
#include <cstring>
#include <type_traits>

#include "core/imei.h"

namespace msg {

struct login {
    core::imei imei; // imei[15] = \0
    uint16_t serial;

    static login make(const char* imei_bytes, uint16_t serial) noexcept;

    bool operator==(const login& other) const { return serial == other.serial && imei == other.imei; }

    login() = default;
};

inline login login::make(const char* imei_bytes, uint16_t serial) noexcept {
    login m;
    m.imei = core::imei::from_bytes(imei_bytes);
    m.serial = serial;
    return m;
}

static_assert(std::is_trivial_v<login>);
static_assert(std::is_trivially_destructible_v<login>);
static_assert(std::is_trivially_destructible_v<login>);

} // namespace msg