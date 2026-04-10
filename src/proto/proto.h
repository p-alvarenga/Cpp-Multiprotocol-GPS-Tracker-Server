#pragma once

#include <cstdint>
#include <memory>

#include "msg/message.h"

namespace proto {

enum class protocol_type {
    gt06 = 0,
    concox,
    tk808,
    // ...
    unknown
};

protocol_type identify_protocol(const uint8_t* data, size_t size) noexcept;

struct decoded_packet {
    uint16_t serial;
    msg::message msg; // decode
};

struct raw_frame {
    uint8_t* data;
    size_t size;
};

class i_framer {
public:
    virtual ~i_framer() = default;
    virtual void feed(const uint8_t* data, size_t size) noexcept = 0;
    virtual bool next(raw_frame& out) noexcept = 0;
};

std::unique_ptr<i_framer> make_framer(protocol_type p);

class i_decoder {
public:
    [[nodiscard("proto::i_decoder::decode() return must not be discarded")]]
    virtual bool decode(const raw_frame& frame, decoded_packet& out) noexcept = 0;
};
std::unique_ptr<i_decoder> make_decoder(protocol_type p);

class i_encoder {
public:
};

struct protocol_vtable {
    i_framer* framer;
    i_decoder* decoder;
    i_encoder* encoder;
};

} // namespace proto