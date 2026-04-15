#pragma once

#include <cstdint>
#include <vector>

#include "protocol/protocol.h"

namespace protocol::gt06 {

inline constexpr int max_framer_offset_size = 1024;

bool match(const protocol::frame& f);

class framer : public protocol::i_framer {
private:
    std::vector<uint8_t> buffer;
    size_t offset{0};

public:
    void feed(const uint8_t* data, size_t size) noexcept final override;
    bool next(protocol::frame& out) noexcept final override;
};

class decoder : public protocol::i_decoder {
private:
    bool decode_imei(core::imei& out, const uint8_t* data, size_t size) const noexcept;
    bool decode_login(msg::login& l, const uint8_t* payload, size_t size) const noexcept;

public:
    bool decode(const frame& f, packet& out) const noexcept final override;
};

class encoder : public protocol::i_encoder {
public:
    void generate_ack(const packet& pkt, frame& out) const noexcept override final;
};

inline decoder g_decoder;
inline encoder g_encoder;

} // namespace protocol::gt06