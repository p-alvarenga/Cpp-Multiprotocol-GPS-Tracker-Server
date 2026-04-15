#include "protocol/gt06/constants.h"
#include "protocol/gt06/gt06.h"
#include "protocol/protocol.h"
#include "protocol/utils/utils.h"

void protocol::gt06::encoder::generate_ack(const protocol::packet& pkt, protocol::frame& out) const noexcept {
    out.data[0] = constants::magic1;
    out.data[1] = constants::magic2;
    out.data[2] = 0x05;
    out.data[3] = pkt.packet_type;
    out.data[4] = (pkt.serial >> 8) & 0xFF;
    out.data[5] = pkt.serial & 0xFF;

    uint16_t crc = utils::calculate_crc16_x25(out.data + 2, 4);
    out.data[6] = (crc >> 8) & 0xFF;
    out.data[7] = crc & 0xFF;

    out.data[8] = constants::stop1;
    out.data[9] = constants::stop2;

    out.size = 10;
}