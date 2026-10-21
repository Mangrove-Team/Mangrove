/*
// Mangrove Project
// APlanCommand.hpp
// Description: Base of every undo command that modifies the plan. Only its subclasses can reach PlanDocument's mutations, so every change of the plan is undoable.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include "PlanDocument.hpp"

#include <QUndoCommand>

#include <cstddef>
#include <optional>

namespace mangrove::app {

    class APlanCommand : public QUndoCommand {
        public:
            void redo() override = 0;
            void undo() override = 0;

        protected:
            explicit APlanCommand(PlanDocument &document, QUndoCommand *parent = nullptr);

            [[nodiscard]] const core::Plan &plan() const noexcept;

            bool insertStand(std::size_t index, core::Stand stand);
            std::optional<core::Stand> removeStand(core::StandId id);
            bool setStandRect(core::StandId id, const core::RectM &rect);

        private:
            PlanDocument &_document;
    }; // class APlanCommand

} // namespace mangrove::app
