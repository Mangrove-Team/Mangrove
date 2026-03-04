/*
// Mangrove Project
// Electricity.h
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_ELECTRICITY_H
#define MANGROVE_ELECTRICITY_H

#include "AConstraint.hpp"

class Electricity : public AConstraint {
public:
    explicit Electricity(float watt);
private:
    float _watt;
};

#endif //MANGROVE_ELECTRICITY_H