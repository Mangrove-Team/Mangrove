/*
// Mangrove Project
// StandTool.cpp
// Description: Draws a new stand by dragging a snapped rectangle.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "tools/stand/StandTool.hpp"
#include "geometry/Geometry.h"
#include "tools/IToolContext.hpp"
#include "tools/PointerEvent.hpp"
#include <optional>

namespace mangrove::app {

    void StandTool::press(IToolContext &context, const PointerEvent &event)
    {
        _anchor = snap(event.position, context.snapStep());
    }

    void StandTool::move(IToolContext &context, const PointerEvent &event)
    {
        if (!_anchor) {
            return;
        }
        const core::RectM rect = rectTo(context, *_anchor, event.position);
        context.setPreview(rect.isEmpty() ? std::nullopt : std::optional(rect));
    }

    void StandTool::release(IToolContext &context, const PointerEvent &event)
    {
        if (!_anchor) {
            return;
        }
        core::RectM rect = rectTo(context, *_anchor, event.position);
        if (rect.isEmpty()) {
            rect = core::RectM{.x = _anchor->x,
                .y = _anchor->y,
                .width = kDefaultSize.x,
                .height = kDefaultSize.y};
        }

        context.setPreview(std::nullopt);
        context.selectStand(context.addStand(rect));
        _anchor.reset();
    }

    void StandTool::cancel(IToolContext &context)
    {
        context.setPreview(std::nullopt);
        _anchor.reset();
    }

    core::RectM StandTool::rectTo(
        const IToolContext &context, core::Vec2 anchor, core::Vec2 position)
    {
        return core::RectM::fromCorners(anchor, snap(position, context.snapStep()));
    }

} // namespace mangrove::app
