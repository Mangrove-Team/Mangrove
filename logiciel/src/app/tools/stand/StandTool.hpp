/*
// Mangrove Project
// StandTool.hpp
// Description: Draws a new stand by dragging a snapped rectangle. A single click places a stand of the default size.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include "tools/ATool.hpp"

#include <optional>

namespace mangrove::app {

    class StandTool final : public ATool {
        public:
            static constexpr core::Vec2 kDefaultSize{4.0, 3.0};

            void press(IToolContext &context, const PointerEvent &event) override;
            void move(IToolContext &context, const PointerEvent &event) override;
            void release(IToolContext &context, const PointerEvent &event) override;
            void cancel(IToolContext &context) override;

        private:
            [[nodiscard]] core::RectM rectTo(const IToolContext &context,
                                             core::Vec2 position) const;

            std::optional<core::Vec2> _anchor;
    }; // class StandTool

} // namespace mangrove::app
