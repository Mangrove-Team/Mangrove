/*
// Mangrove Project
// SelectTool.cpp
// Description: Selects the stand under the pointer and drags it, snapped.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "tools/select/SelectTool.hpp"
#include "plan/Ids.h"
#include "plan/Stand.h"
#include "tools/IToolContext.hpp"
#include "tools/PointerEvent.hpp"

namespace mangrove::app {

    void SelectTool::press(IToolContext &context, const PointerEvent &event)
    {
        const core::StandId standId = context.plan().standAt(event.position);
        context.selectStand(standId);

        const core::Stand *stand = context.plan().find(standId);
        if (stand == nullptr) {
            return;
        }
        const core::Vec2 origin = stand->rect.topLeft();
        _drag = Drag{.stand = standId, .grab = event.position - origin, .origin = origin};
    }

    void SelectTool::move(IToolContext &context, const PointerEvent &event)
    {
        if (_drag) {
            context.moveStand(_drag->stand, snap(event.position - _drag->grab, context.snapStep()));
        }
    }

    void SelectTool::release(IToolContext &context, const PointerEvent &event)
    {
        move(context, event);
        _drag.reset();
    }

    void SelectTool::cancel(IToolContext &context)
    {
        if (_drag) {
            context.moveStand(_drag->stand, _drag->origin);
        }
        _drag.reset();
    }

} // namespace mangrove::app
