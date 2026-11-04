/*
// Mangrove Project
// CommandIds.hpp
// Description: QUndoCommand::id() values. Commands sharing an id may merge.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

namespace mangrove::app {

    enum class CommandId : int {
        MoveStand = 1,
    };

} // namespace mangrove::app
