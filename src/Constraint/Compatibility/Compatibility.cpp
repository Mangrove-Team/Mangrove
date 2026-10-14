/*
// Mangrove Project
// Compatibility.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Compatibility.hpp"

Compatibility::Compatibility() {
    _name = "Compatibility";
}

std::string Compatibility::getDataInfo() const {
    std::string dataInfo;

    for (const auto& stands: _incompatibleStands) {
        dataInfo += stands->getName() + ", ";
    }
    if (!dataInfo.empty()) {
        dataInfo.pop_back();
        dataInfo.pop_back();
    }
    return dataInfo;
}

const std::list<std::shared_ptr<Stand>>& Compatibility::getIncompatibleStands() const {
    return _incompatibleStands;
}

void Compatibility::addIncompatibleStand(const std::shared_ptr<Stand>& stand) {
    _incompatibleStands.push_back(stand);
}
