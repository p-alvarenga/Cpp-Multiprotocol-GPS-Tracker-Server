#include "core/log.h"
#include "protocol/gt06/constants.h"
#include "protocol/gt06/gt06.h"
#include "protocol/protocol.h"

bool protocol::gt06::decoder::decode(const frame& f, packet& out) const noexcept {
    core::log::hex(f.data, f.size);

    if (f.size < 15) [[__unlikely__]] {
        core::log::err("gt06::decoder: frame size less than 15 bytes");
        return false;
    }

    if (!match(f)) [[__unlikely__]] {
        core::log::err("gt06::decoder: frame does not match");
        return false;
    }

    int len = (int)f.data[2];
    uint8_t pkt_type = f.data[3];

    if (len + 5 != (int)f.size) {
        core::log::err("gt06: packet size does not match expected");
        return false;
    }

    switch (pkt_type) {

    case constants::login_id:
        msg::login l;

        if (!decode_login(l, f.data + 4, len - 5)) {
            core::log::err("gt06: could not decode login packet");
            return false;
        }

        out.msg.set_login(l);

        return true;

    default:
        core::log::err("gt06: could not find packet type %02X", pkt_type);
        return false;
    }

    return true;
}

bool protocol::gt06::decoder::decode_login(msg::login& l, const uint8_t* payload, size_t size) const noexcept {
    if (size != 8) {
        core::log::err("gt06::decoder: BCD IMEI has unexpected size");
        return false;
    }

    return decode_imei(l.imei, payload, size);
}