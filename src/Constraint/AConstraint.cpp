/*
// Mangrove Project
// AConstraint.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "AConstraint.hpp"

AConstraint::AConstraint() {
    _type = Constraint::None;
}

std::string AConstraint::getName() const {
    return _name;
}

Constraint::Type AConstraint::getType() const {
    return _type;
}