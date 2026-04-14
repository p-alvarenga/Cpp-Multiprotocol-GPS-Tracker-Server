#include "protocol/utils/utils.h"

#include <cstdint>
#include <unistd.h>

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