/*
// Mangrove Project
// APlanCommand.cpp
// Description: Base of every undo command that modifies the plan.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "commands/APlanCommand.hpp"
#include "PlanDocument.hpp"
#include "geometry/Geometry.h"
#include "plan/Ids.h"
#include "plan/Plan.h"
#include "plan/Stand.h"

#include <cstddef>
#include <optional>
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

    std::optional<core::Stand> APlanCommand::removeStand(core::StandId standId)
    {
        return _document.removeStand(standId);
    }

    bool APlanCommand::setStandRect(core::StandId standId, const core::RectM &rect)
    {
        return _document.setStandRect(standId, rect);
    }

} // namespace mangrove::app
