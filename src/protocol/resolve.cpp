#include "protocol/gt06/constants.h"
#include "protocol/gt06/gt06.h"
#include "protocol/protocol.h"

protocol::type protocol::identify_protocol(const uint8_t* data, size_t size) noexcept {
    if (size < 2) return type::unknown;

    for (size_t i = 0; i + 1 < size; ++i) {
        // gt06
        if (data[i] == gt06::constants::magic1 && data[i + 1] == gt06::constants::magic2) {
            return type::gt06;
        }
    }

    return type::unknown;
}

const protocol::descriptor* protocol::registry::resolve(protocol::type p) {
    if (p == type::unknown) return nullptr;

    switch (p) {
    case type::gt06:
        static const descriptor desc{
            &gt06::g_decoder,
            &gt06::g_encoder,
            []() -> std::unique_ptr<i_framer> { return std::make_unique<gt06::framer>(); },
        };
        return &desc;

    default:
        return nullptr;
    }
}