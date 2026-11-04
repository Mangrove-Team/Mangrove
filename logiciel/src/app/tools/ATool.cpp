/*
// Mangrove Project
// ATool.cpp
// Description: Base of every tool.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "tools/ATool.hpp"
#include "geometry/Geometry.h"
#include "tools/IToolContext.hpp"
#include "tools/PointerEvent.hpp"

#include <cmath>

namespace mangrove::app {

    void ATool::move(IToolContext & /*context*/, const PointerEvent & /*event*/)
    { }

    void ATool::release(IToolContext & /*context*/, const PointerEvent & /*event*/)
    { }

    void ATool::cancel(IToolContext & /*context*/)
    { }

    double ATool::snap(double value, double step)
    {
        return step > 0.0 ? std::round(value / step) * step : value;
    }

    core::Vec2 ATool::snap(core::Vec2 point, double step)
    {
        return {.x = snap(point.x, step), .y = snap(point.y, step)};
    }

} // namespace mangrove::app
