/*
// Mangrove Project
// Plan.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Plan.hpp"

std::list<Stand> Plan::getStandsByConstraint(Constraint::Type type) const {
    std::list<Stand> stands;

    for (const auto& elt : _stands) {
        stands.push_back(elt);
    }
    return stands;
}
