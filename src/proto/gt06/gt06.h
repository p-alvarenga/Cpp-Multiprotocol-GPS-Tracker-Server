#pragma once

#include <cstdint>
#include <vector>

#include "proto/proto.h"

namespace proto::gt06 {

inline constexpr int max_framer_offset_size = 1024;

bool match(const proto::raw_frame& f);
bool decode_imei(core::imei& out, const uint8_t* data, size_t size);

class framer : public proto::i_framer {
private:
    std::vector<uint8_t> buffer;
    size_t offset{0};

public:
    void feed(const uint8_t* data, size_t size) noexcept final override;
    bool next(proto::raw_frame& out) noexcept final override;
};

class decoder : public proto::i_decoder {
private:
    bool decode_login(msg::login& l, const uint8_t* payload, size_t size) const noexcept;
    // ...

public:
    bool decode(const raw_frame& frame, packet& out) const noexcept final override;
};

class encoder : public proto::i_encoder {};

inline decoder g_decoder;
inline encoder g_encoder;

} // namespace proto::gt06