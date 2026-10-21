/*
// Mangrove Project
// APlanCommand.cpp
// Description: Base of every undo command that modifies the plan.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "commands/APlanCommand.hpp"

#include <utility>

namespace mangrove::app {

    APlanCommand::APlanCommand(PlanDocument &document, QUndoCommand *parent)
        : QUndoCommand(parent), _document(document)
    { }

    const core::Plan &APlanCommand::plan() const noexcept
    {
        return _document.plan();
    }

    bool APlanCommand::insertStand(std::size_t index, core::Stand stand)
    {
        return _document.insertStand(index, std::move(stand));
    }

    std::optional<core::Stand> APlanCommand::removeStand(core::StandId id)
    {
        return _document.removeStand(id);
    }

    bool APlanCommand::setStandRect(core::StandId id, const core::RectM &rect)
    {
        return _document.setStandRect(id, rect);
    }

} // namespace mangrove::app
