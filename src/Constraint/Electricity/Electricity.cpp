/*
// Mangrove Project
// Electricity.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Electricity.hpp"

Electricity::Electricity(const float watt): _watt(watt) {
    _name = "Electricity";
}

float Electricity::getWatt() const {
    return _watt;
}

std::string Electricity::getDataInfo() const {
    return std::to_string(_watt) + "W";
}
