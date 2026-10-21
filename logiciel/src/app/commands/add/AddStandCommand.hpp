/*
// Mangrove Project
// AddStandCommand.hpp
// Description: Adds a stand on top of the others. The stand's id is fixed at construction, so redo always brings back the same stand.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include <QCoreApplication>

#include "commands/APlanCommand.hpp"

namespace mangrove::app {

    class AddStandCommand final : public APlanCommand {

            Q_DECLARE_TR_FUNCTIONS(AddStandCommand)

        public:
            AddStandCommand(PlanDocument &document, const core::RectM &rect);

            [[nodiscard]] core::StandId standId() const noexcept;

            void redo() override;
            void undo() override;

        private:
            core::Stand _stand;
    }; // class AddStandCommand

} // namespace mangrove::app
