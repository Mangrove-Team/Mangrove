/*
// Mangrove Project
// ATool.cpp
// Description: Base of every tool.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "tools/ATool.hpp"

#include <cmath>

namespace mangrove::app {

    void ATool::move(IToolContext &, const PointerEvent &)
    { }

    void ATool::release(IToolContext &, const PointerEvent &)
    { }

    void ATool::cancel(IToolContext &)
    { }

    double ATool::snap(double value, double step)
    {
        return step > 0.0 ? std::round(value / step) * step : value;
    }

    core::Vec2 ATool::snap(core::Vec2 point, double step)
    {
        return {snap(point.x, step), snap(point.y, step)};
    }

} // namespace mangrove::app
