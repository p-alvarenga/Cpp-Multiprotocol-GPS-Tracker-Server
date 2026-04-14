#pragma once

#include <cstring>

namespace core {

class imei {
private:
    char data[16];

public:
    static imei make_from_bytes(const char* src) noexcept {
        imei i;

        memcpy(i.data, src, 15);
        i.data[15] = '\0';

        return i;
    }

    bool operator==(const imei& other) const noexcept { return memcmp(data, other.data, 15); }
    bool is_valid() { return true; }

    const char* get() const noexcept { return data; }

    imei() = default;
};
} // namespace core