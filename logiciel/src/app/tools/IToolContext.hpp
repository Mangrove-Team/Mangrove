/*
// Mangrove Project
// IToolContext.hpp
// Description: Interface of everything a tool can see and do. Implemented by Editor; a test can implement it to drive a tool on its own.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include "geometry/Geometry.h"
#include "plan/Ids.h"
#include "plan/Plan.h"

#include <optional>

namespace mangrove::app {

    class IToolContext {
        public:
            virtual ~IToolContext() = default;
            IToolContext(const IToolContext &) = delete;
            IToolContext &operator=(const IToolContext &) = delete;
            IToolContext(IToolContext &&) = delete;
            IToolContext &operator=(IToolContext &&) = delete;

            [[nodiscard]] virtual const core::Plan &plan() const = 0;
            // Grid step, in meters, that tools snap positions to.
            [[nodiscard]] virtual double snapStep() const = 0;
            [[nodiscard]] virtual core::StandId selectedStand() const = 0;

            virtual void selectStand(core::StandId standId) = 0;
            // Rectangle shown over the plan during a gesture; nullopt hides it.
            virtual void setPreview(std::optional<core::RectM> rect) = 0;

            // Every edit below is pushed as an undoable command.
            // Returns an invalid id when nothing was added.
            virtual core::StandId addStand(const core::RectM &rect) = 0;
            virtual void moveStand(core::StandId standId, core::Vec2 topLeft) = 0;

        protected:
            IToolContext() = default;
    }; // class IToolContext

} // namespace mangrove::app
