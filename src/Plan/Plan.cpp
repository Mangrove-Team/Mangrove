/*
// Mangrove Project
// Plan.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Plan.hpp"

std::list<std::shared_ptr<Stand>> Plan::getStandsByConstraint(const Constraint::Type type) const {
    std::list<std::shared_ptr<Stand>> stands;

    for (const auto& stand : _stands)
        if (auto constraints = stand->getConstraints(); constraints.contains(type))
            stands.push_back(stand);
    return stands;
}

void Plan::addStand(const Stand& stand) {
    _stands.push_back(std::make_shared<Stand>(stand));
}