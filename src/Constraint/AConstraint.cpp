/*
// Mangrove Project
// AConstraint.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "AConstraint.hpp"

AConstraint::AConstraint():
    _type(std::type_index(typeid(*this))) {}

const std::string& AConstraint::getName() const {
    return _name;
}

std::type_index AConstraint::getType() const {
    return typeid(*this);
}

std::string AConstraint::getDataInfo() const {
    return "";
}

std::string AConstraint::check() const {
    return "";
}
