/*
// Mangrove Project
// MoveStandCommand.cpp
// Description: Moves a stand; a whole drag is a single undo step.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "commands/move/MoveStandCommand.hpp"
#include "commands/CommandIds.hpp"

#include "Conversions.hpp"

namespace mangrove::app {

    MoveStandCommand::MoveStandCommand(PlanDocument &document, core::StandId stand,
                                       const core::RectM &from, const core::RectM &to,
                                       std::uint64_t gesture)
        : APlanCommand(document), _stand(stand), _from(from), _to(to), _gesture(gesture)
    {
        const core::Stand *found = document.plan().find(stand);
        setText(tr("Move stand %1").arg(found ? toQt(found->label) : QString()));
    }

    int MoveStandCommand::id() const
    {
        return static_cast<int>(CommandId::MoveStand);
    }

    bool MoveStandCommand::mergeWith(const QUndoCommand *other)
    {
        const auto *next = static_cast<const MoveStandCommand *>(other);
        if (next->_stand != _stand || next->_gesture != _gesture)
            return false;
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
