/*
// Mangrove Project
// AddStandCommand.cpp
// Description: Adds a stand on top of the others.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "commands/add/AddStandCommand.hpp"

#include "Conversions.hpp"

namespace mangrove::app {

    AddStandCommand::AddStandCommand(PlanDocument &document, const core::RectM &rect)
        : APlanCommand(document), _stand(document.makeStand(rect))
    {
        setText(tr("Add stand %1").arg(toQt(_stand.label)));
    }

    core::StandId AddStandCommand::standId() const noexcept
    {
        return _stand.id;
    }

    void AddStandCommand::redo()
    {
        insertStand(plan().size(), _stand);
    }

    void AddStandCommand::undo()
    {
        removeStand(_stand.id);
    }

} // namespace mangrove::app
