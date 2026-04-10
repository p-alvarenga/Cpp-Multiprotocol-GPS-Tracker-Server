#pragma once

#include <cstdint>

namespace proto::gt06::constants {

inline constexpr uint8_t magic1 = 0x78;
inline constexpr uint8_t magic2 = 0x78;

inline constexpr uint8_t stop1 = 0x0D;
inline constexpr uint8_t stop2 = 0x0A;

inline constexpr uint8_t login_id = 0x01;
inline constexpr uint8_t location_id = 0x12;

} // namespace proto::gt06::constants