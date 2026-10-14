/*
// Mangrove Project
// Stand.h
// Description: A stand placed on the plan.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include "geometry/Geometry.h"
#include "plan/Ids.h"

#include <string>

namespace mangrove::core {

struct Stand {
    StandId id;
    std::string label;
    RectM rect;

    friend bool operator==(const Stand &, const Stand &) = default;
};

} // namespace mangrove::core
