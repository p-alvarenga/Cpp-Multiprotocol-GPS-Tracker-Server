#pragma once

#include <cstdint>
#include <memory>

#include "msg/message.h"

namespace protocol {

enum class type {
    gt06 = 0,
    concox,
    tk808,
    // ...
    unknown
};

type identify_protocol(const uint8_t* data, size_t size) noexcept;

struct packet {
    uint16_t serial;
    msg::message msg;
    packet() = default;
};

struct frame {
    uint8_t* data;
    size_t size;
};

class i_framer {
public:
    virtual ~i_framer() = default;
    virtual void feed(const uint8_t* data, size_t size) noexcept = 0;
    virtual bool next(frame& out) noexcept = 0;
};

class i_decoder {
public:
    [[nodiscard("proto::i_decoder::decode() return must not be discarded")]]
    virtual bool decode(const frame& in, packet& out) const noexcept = 0;
};

class i_encoder {
public:
    frame generate_ack(const packet& pkt, frame& out);
};

struct protocol_descriptor {
    i_decoder* decoder;
    i_encoder* encoder;
    std::unique_ptr<i_framer> (*make_framer)();
};

namespace registry {
const protocol_descriptor* resolve(protocol::type p);
}
} // namespace protocol