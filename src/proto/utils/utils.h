#pragma once

#include <cstddef>
#include <cstdint>
#include <unistd.h>

namespace proto::utils {

ssize_t decode_bcd(char* out, const uint8_t* data, size_t size) noexcept;

// crc related

} // namespace proto::utils