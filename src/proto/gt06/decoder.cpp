#include "core/log.h"
#include "proto/gt06/constants.h"
#include "proto/gt06/gt06.h"
#include "proto/proto.h"

bool proto::gt06::decoder::decode(const raw_frame& frame, decoded_packet& out) noexcept {
    core::log::hex(frame.data, frame.size);

    if (frame.size < 15) [[__unlikely__]] {
        core::log::err("gt06::decoder: frame size less than 15 bytes");
        return false;
    }

    if (!match(frame)) [[__unlikely__]] {
        core::log::err("gt06::decoder: frame does not match");
        return false;
    }

    int len = (int)frame.data[2];
    uint8_t pkt_type = frame.data[3];

    if (len + 5 != (int)frame.size) return false;

    switch (pkt_type) {

    case constants::login_id:
        msg::login l;

        if (!decode_login(l, frame.data + 4, len - 5)) return false;
        out.msg.set_login(l);

        return true;

    default:
        return false;
    }

    return true;
}

bool proto::gt06::decoder::decode_login(msg::login& l, const uint8_t* payload, size_t size) const noexcept {
    if (size != 8) {
        core::log::err("gt06::decoder: BCD IMEI has unexpected size");
        return false;
    }

    return decode_imei(l.imei, payload, size);
}