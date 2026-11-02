/*
// Mangrove Project
// Ids.h
// Description: Strong identifier types. A default-constructed id is invalid.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include <cstdint>
#include <functional>

namespace mangrove::core {

struct StandId {
    std::uint64_t value = 0;

    constexpr explicit operator bool() const noexcept { return value != 0; }
    friend constexpr bool operator==(StandId, StandId) noexcept = default;
};

} // namespace mangrove::core

template<>
struct std::hash<mangrove::core::StandId> {
    std::size_t operator()(mangrove::core::StandId id) const noexcept
    {
        return std::hash<std::uint64_t>{}(id.value);
    }
};
