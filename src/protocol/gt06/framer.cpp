#include "protocol/gt06/constants.h"
#include "protocol/gt06/gt06.h"

void protocol::gt06::framer::feed(const uint8_t* data, size_t size) noexcept {
    buffer.insert(buffer.end(), data, data + size);
}

bool protocol::gt06::framer::next(frame& out) noexcept {
    size_t buffer_size = buffer.size();

    if (buffer_size < 18) return false;

    if (offset > max_framer_offset_size && offset > buffer_size / 2) {
        buffer.erase(buffer.begin(), buffer.begin() + offset);
        offset = 0;
        buffer_size = buffer.size();
    }

    size_t it = offset;

    for (; it + 1 < buffer_size; ++it) {
        if (buffer[it] == constants::magic1 && constants::magic2) {
            break;
        }
    }

    if (it + 1 >= buffer_size) [[unlikely]] {
        if (buffer_size > 0) {
            offset = buffer_size - 1;
        }
        return false;
    }

    offset = it;
    if (buffer_size - offset < 3) return false;

    uint8_t len = buffer[offset + 2];
    size_t total = len + 5;

    if (buffer_size - offset < total) return false;

    if (buffer[offset + total - 2] != constants::stop1 || buffer[offset + total - 1] != constants::stop2) {
        offset++;
        return false;
    }

    out.data = buffer.data() + offset;
    out.size = total;
    offset += total;

    return true;
}