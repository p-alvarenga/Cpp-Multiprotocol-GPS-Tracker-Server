#include "proto/gt06/constants.h"
#include "proto/gt06/gt06.h"
#include "proto/proto.h"

proto::protocol_type proto::identify_protocol(const uint8_t* data, size_t size) noexcept {
    if (size < 2) return protocol_type::unknown;

    for (size_t i = 0; i + 1 < size; ++i) {
        // gt06
        if (data[i] == proto::gt06::constants::magic1 && data[i + 1] == proto::gt06::constants::magic2) {
            return protocol_type::gt06;
        }
    }

    return protocol_type::unknown;
}

const proto::protocol_descriptor* proto::registry::resolve(proto::protocol_type p) {
    if (p == protocol_type::unknown) return nullptr;

    switch (p) {
    case protocol_type::gt06:
        static const protocol_descriptor desc{
            &gt06::g_decoder,
            &gt06::g_encoder,
            []() -> std::unique_ptr<i_framer> { return std::make_unique<gt06::framer>(); },
        };
        return &desc;

    default:
        return nullptr;
    }
}