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
        // ...
    }

    return protocol_type::unknown;
}

std::unique_ptr<proto::i_framer> proto::make_framer(proto::protocol_type p) {
    if (p == protocol_type::unknown) return nullptr;

    switch (p) {
    case proto::protocol_type::gt06:
        return std::make_unique<proto::gt06::framer>();

        // case ...

    default:
        return nullptr;
    }
}

std::unique_ptr<proto::i_decoder> proto::make_decoder(proto::protocol_type p) {
    if (p == protocol_type::unknown) return nullptr;

    switch (p) {
    case proto::protocol_type::gt06:
        return std::make_unique<proto::gt06::decoder>();

        // case ...

    default:
        return nullptr;
    }
}