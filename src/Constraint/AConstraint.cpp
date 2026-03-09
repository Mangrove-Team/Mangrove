/*
// Mangrove Project
// AConstraint.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "AConstraint.hpp"

AConstraint::AConstraint(): _type(std::type_index(typeid(*this))){}

std::string& AConstraint::getName() {
    return _name;
}

std::type_index AConstraint::getType() const {
    return _type;
}