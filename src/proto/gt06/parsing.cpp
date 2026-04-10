#include "proto/gt06/constants.h"
#include "proto/gt06/gt06.h"
#include "proto/utils/utils.h"

bool proto::gt06::match(const proto::raw_frame& f) {
    if (f.size < 15) return false;

    return f.data[0] == constants::magic1 && f.data[1] == constants::magic2 && f.data[f.size - 2] == constants::stop1 &&
           f.data[f.size - 1] == constants::stop2;
}

bool proto::gt06::decode_imei(core::imei& out, const uint8_t* data, size_t size) {
    if (size != 8) return false;

    char buf[32];
    int n = (int)proto::utils::decode_bcd(buf, data, size);

    if (n != 15) return false;

    out.from_bytes(buf);

    return true;
}