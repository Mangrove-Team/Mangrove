/*
// Mangrove Project
// SelectTool.cpp
// Description: Selects the stand under the pointer and drags it, snapped.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "tools/select/SelectTool.hpp"

namespace mangrove::app {

    void SelectTool::press(IToolContext &context, const PointerEvent &event)
    {
        const core::StandId id = context.plan().standAt(event.position);
        context.selectStand(id);

        const core::Stand *stand = context.plan().find(id);
        if (!stand)
            return;
        const core::Vec2 origin = stand->rect.topLeft();
        _drag = Drag{id, event.position - origin, origin};
    }

    void SelectTool::move(IToolContext &context, const PointerEvent &event)
    {
        if (_drag)
            context.moveStand(_drag->stand, snap(event.position - _drag->grab, context.snapStep()));
    }

    void SelectTool::release(IToolContext &context, const PointerEvent &event)
    {
        move(context, event);
        _drag.reset();
    }

    void SelectTool::cancel(IToolContext &context)
    {
        if (_drag)
            context.moveStand(_drag->stand, _drag->origin);
        _drag.reset();
    }

} // namespace mangrove::app
