/*
// Mangrove Project
// Stand.cpp
// Description: Stand class that represents a stand in a plan and contains its constraints
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Stand.hpp"
#include "Transform/Transform.hpp"

Stand::Stand(std::string name): _name(std::move(name)) {
    addConstraint<Transform>();
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
