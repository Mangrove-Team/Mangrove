/*
// Mangrove Project
// PointerEvent.hpp
// Description: A pointer event as tools receive it: already converted to meters.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include "geometry/Geometry.h"

namespace mangrove::app {

    struct PointerEvent {
            core::Vec2 position;
    }; // struct PointerEvent

} // namespace mangrove::app
