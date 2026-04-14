#include "protocol/gt06/constants.h"
#include "protocol/gt06/gt06.h"
#include "protocol/utils/utils.h"

bool protocol::gt06::match(const frame& f) {
    if (f.size < 15) return false;

    return f.data[0] == constants::magic1 && f.data[1] == constants::magic2 && f.data[f.size - 2] == constants::stop1 &&
           f.data[f.size - 1] == constants::stop2;
}

bool protocol::gt06::decoder::decode_imei(core::imei& out, const uint8_t* data, size_t size) const noexcept {
    if (size != 8) return false;

    char buf[32];

    int n = utils::decode_bcd(buf, data, size);
    if (n != 15) return false;

    out = core::imei::make_from_bytes(buf);

    return true;
}