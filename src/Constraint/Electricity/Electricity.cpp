/*
// Mangrove Project
// Electricity.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Electricity.hpp"

Electricity::Electricity(const float watt) {
    _watt = watt;
    _name = "Electricity";
}

float Electricity::getWatt() const {
    return _watt;
}