/*
// Mangrove Project
// Plan.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Plan.hpp"

const std::list<std::shared_ptr<Stand>>& Plan::getStands() const {
    return _stands;
}

void Plan::addStand(const std::shared_ptr<Stand>& stand) {
    _stands.push_back(stand);
}
