/*
// Mangrove Project
// Stand.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Stand.hpp"

void Stand::addConstraint(const AConstraint& c) {
    _constraints.emplace(c.getType(), c);
}

std::string Stand::getName() const {
    return _name;
}

AConstraint Stand::getConstraintByType(const Constraint::Type type) const {
    return _constraints.at(type);
}
