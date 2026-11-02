/*
// Mangrove Project
// ATool.hpp
// Description: Base of every tool. Ignores the events a tool does not override, so a new tool only implements press() and what it needs; provides grid snapping.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include "tools/ITool.hpp"

namespace mangrove::app {

    class ATool : public ITool {
        public:
            void move(IToolContext &context, const PointerEvent &event) override;
            void release(IToolContext &context, const PointerEvent &event) override;
            void cancel(IToolContext &context) override;

        protected:
            ATool() = default;

            // Nearest multiple of `step`; a step <= 0 disables snapping.
            [[nodiscard]] static double snap(double value, double step);
            [[nodiscard]] static core::Vec2 snap(core::Vec2 point, double step);
    }; // class ATool

} // namespace mangrove::app
