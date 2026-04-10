#pragma once

#include <cstring>

namespace core {

class imei {
private:
    char data[15];

public:
    static imei from_bytes(const char* src) noexcept {
        imei i;
        memcpy(i.data, src, 15);
        return i;
    }

    bool operator==(const imei& other) const noexcept { return memcmp(data, other.data, 15); }
    bool is_valid() { return true; }
};
} // namespace core