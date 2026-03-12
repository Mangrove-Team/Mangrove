/*
// Mangrove Project
// Electricity.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Electricity.hpp"

namespace {
[[maybe_unused]] const bool electricityRegistered = AConstraint::registerType("Electricity", [] {
    return std::make_shared<Electricity>();
});
}

Electricity::Electricity(const float watt) {
    _watt = watt;
    _name = "Electricity";
}

float Electricity::getWatt() const {
    return _watt;
}