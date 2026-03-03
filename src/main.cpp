/*
// Mangrove Project
// main.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include <iostream>

#include "Stand.hpp"
#include "Electricity/Electricity.hpp"
#include "PmrAccess/PmrAccess.hpp"

auto main() -> int {
    Stand stand;
    Electricity electricity;
    PmrAccess pmrAccess;

    stand.addConstraint(electricity);
    stand.addConstraint(pmrAccess);

    std::cout << stand.getConstraintByType(Constraint::Electricity).getName() << std::endl;
    std::cout << stand.getConstraintByType(Constraint::PMRAccess).getName() << std::endl;
}
