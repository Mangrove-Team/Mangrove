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
    _type = Constraint::Electricity;
}
