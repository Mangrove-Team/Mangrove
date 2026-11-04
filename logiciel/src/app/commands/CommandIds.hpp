/*
// Mangrove Project
// CommandIds.hpp
// Description: QUndoCommand::id() values. Commands sharing an id may merge.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include <cstdint>

namespace mangrove::app {

    enum class CommandId : std::uint8_t {
        MoveStand = 1,
    };

} // namespace mangrove::app
