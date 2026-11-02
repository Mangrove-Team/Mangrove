/*
// Mangrove Project
// PlanDocument.hpp
// Description: Owns the plan and its undo history. The plan can only be modified by undo commands (see commands/APlanCommand.hpp), so every change is undoable and announced through the signals.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include "plan/Plan.h"

#include <QObject>
#include <QUndoStack>

#include <cstddef>
#include <optional>

namespace mangrove::app {

    class PlanDocument : public QObject {

            Q_OBJECT

        public:
            explicit PlanDocument(QObject *parent = nullptr);

            [[nodiscard]] const core::Plan &plan() const noexcept { return _plan; }
            [[nodiscard]] QUndoStack *undoStack() noexcept { return &_undoStack; }
            [[nodiscard]] const QUndoStack *undoStack() const noexcept { return &_undoStack; }

            // Reserves an id and a label for a stand that a command will insert later.
            [[nodiscard]] core::Stand makeStand(const core::RectM &rect)
            {
                return _plan.makeStand(rect);
            }

        signals:
            void aboutToInsertStand(int index);
            void standInserted(int index);
            void aboutToRemoveStand(int index);
            void standRemoved(int index);
            void standChanged(int index);
            // Emitted after any change, once the fine-grained signals above are done.
            void planChanged();

        private:
            friend class APlanCommand;

            bool insertStand(std::size_t index, core::Stand stand);
            std::optional<core::Stand> removeStand(core::StandId id);
            bool setStandRect(core::StandId id, const core::RectM &rect);

            core::Plan _plan;
            QUndoStack _undoStack;
    }; // class PlanDocument

} // namespace mangrove::app
