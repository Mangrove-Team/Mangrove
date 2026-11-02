/*
// Mangrove Project
// StandTool.hpp
// Description: Draws a new stand by dragging a snapped rectangle. A single click places a stand of the default size.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include "geometry/Geometry.h"
#include "tools/ATool.hpp"
#include "tools/IToolContext.hpp"
#include "tools/PointerEvent.hpp"

#include <optional>

namespace mangrove::app {

    class StandTool final : public ATool {
        public:
            static constexpr core::Vec2 kDefaultSize{.x = 4.0, .y = 3.0};

            void press(IToolContext &context, const PointerEvent &event) override;
            void move(IToolContext &context, const PointerEvent &event) override;
            void release(IToolContext &context, const PointerEvent &event) override;
            void cancel(IToolContext &context) override;

        private:
            [[nodiscard]] static core::RectM rectTo(
                const IToolContext &context, core::Vec2 anchor, core::Vec2 position);

            std::optional<core::Vec2> _anchor;
    }; // class StandTool

} // namespace mangrove::app
