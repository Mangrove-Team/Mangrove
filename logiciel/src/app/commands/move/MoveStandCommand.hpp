/*
// Mangrove Project
// MoveStandCommand.hpp
// Description: Moves a stand. Commands of the same gesture on the same stand merge, so a whole drag is a single undo step.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include "commands/APlanCommand.hpp"

#include <QCoreApplication>

#include <cstdint>

namespace mangrove::app {

    class MoveStandCommand final : public APlanCommand {

            Q_DECLARE_TR_FUNCTIONS(MoveStandCommand)

        public:
            MoveStandCommand(PlanDocument &document, core::StandId stand, const core::RectM &from,
                             const core::RectM &to, std::uint64_t gesture);

            [[nodiscard]] int id() const override;

            bool mergeWith(const QUndoCommand *other) override;

            void redo() override;
            void undo() override;

        private:
            core::StandId _stand;
            core::RectM _from;
            core::RectM _to;
            std::uint64_t _gesture;
    }; // class MoveStandCommand

} // namespace mangrove::app
