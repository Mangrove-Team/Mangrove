/*
// Mangrove Project
// Stand.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Stand.hpp"

std::string& Stand::getName() {
    return _name;
}

std::map<Constraint::Type, std::shared_ptr<AConstraint>>& Stand::getConstraints() {
    return _constraints;
}

std::shared_ptr<AConstraint> Stand::getConstraintByType(const Constraint::Type type) const {
    return _constraints.at(type);
}

void Stand::setName(const std::string &name) {
    _name = name;
}