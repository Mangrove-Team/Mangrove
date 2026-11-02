/*
// Mangrove Project
// MoveStandCommand.cpp
// Description: Moves a stand; a whole drag is a single undo step.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "commands/move/MoveStandCommand.hpp"
#include "PlanDocument.hpp"
#include "commands/APlanCommand.hpp"
#include "commands/CommandIds.hpp"

#include "Conversions.hpp"
#include "geometry/Geometry.h"
#include "plan/Ids.h"
#include "plan/Stand.h"
#include <cstdint>

namespace mangrove::app {

    MoveStandCommand::MoveStandCommand(PlanDocument &document, core::StandId stand,
        const core::RectM &from, core::Vec2 topLeft, std::uint64_t gesture)
        : APlanCommand(document), _stand(stand), _from(from), _to(from.movedTo(topLeft)),
          _gesture(gesture)
    {
        const core::Stand *found = document.plan().find(stand);
        setText(tr("Move stand %1").arg((found != nullptr) ? toQt(found->label) : QString()));
    }

    int MoveStandCommand::id() const
    {
        return static_cast<int>(CommandId::MoveStand);
    }

    bool MoveStandCommand::mergeWith(const QUndoCommand *other)
    {
        const auto *next = dynamic_cast<const MoveStandCommand *>(other);
        if (next == nullptr || next->_stand != _stand || next->_gesture != _gesture) {
            return false;
        }
        _to = next->_to;
        setObsolete(_to == _from);
        return true;
    }

    void MoveStandCommand::redo()
    {
        setStandRect(_stand, _to);
    }

    void MoveStandCommand::undo()
    {
        setStandRect(_stand, _from);
    }

} // namespace mangrove::app
