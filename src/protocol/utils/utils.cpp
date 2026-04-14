#include "protocol/utils/utils.h"

#include <cstdint>
#include <unistd.h>

#include "core/log.h"

[[__nodiscard__("proto::utils::decode_bcd() return must not be discarded")]]
ssize_t protocol::utils::decode_bcd(char* out, const uint8_t* data, size_t size) noexcept {
    if (size == 0) return -1;

    size_t cursor = 0;

    for (size_t i = 0; i < size; i++) {
        uint8_t b = data[i];

        uint8_t high = (b >> 4) & 0x0F;
        uint8_t low = b & 0x0F;

        if (high > 9) return -1;
        out[cursor++] = static_cast<char>('0' + high);

        if (i == size - 1 && low == 0x0F) return cursor;
        if (low > 9) return -1;

        out[cursor++] = static_cast<char>('0' + low);
    }

    return cursor;
}

uint16_t protocol::utils::calculate_crc16_x25(const uint8_t* payload, size_t size) noexcept {
    core::log::info("utils: calculating crc16_x25...");
    core::log::hex(payload, size);

    uint16_t crc = 0xFFFF;

    for (size_t i = 0; i < size; i++) {
        crc ^= payload[i];

        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0x8408;
            } else {
                crc >>= 1;
            }
        }
    }

    return ~crc;
}