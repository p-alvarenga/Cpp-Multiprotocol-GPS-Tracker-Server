#pragma once

#include <cstddef>
#include <cstdint>
#include <unistd.h>

namespace protocol::utils {

ssize_t decode_bcd(char* out, const uint8_t* data, size_t size) noexcept;
uint16_t calculate_crc16_x25(const uint8_t* payload, size_t size) noexcept;

// crc related

} // namespace protocol::utils