/*
// Mangrove Project
// Stand.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Stand.hpp"
#include "Space.hpp"

Stand::Stand(std::string name): _name(std::move(name)) {
    addConstraint<Space>();
}

const std::string& Stand::getName() const {
    return _name;
}

const std::map<std::type_index, std::shared_ptr<AConstraint>>& Stand::getConstraints() const {
    return _constraints;
}

void Stand::setName(const std::string& name) {
    _name = name;
}
