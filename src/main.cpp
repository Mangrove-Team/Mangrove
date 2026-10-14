/*
// Mangrove Project
// main.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include <ostream>
#include <iostream>

#include "Plan.hpp"
#include "Space.hpp"
#include "Electricity.hpp"
#include "Compatibility.hpp"

int main() {
    const auto plan = std::make_shared<Plan>();
    const auto stand1 = std::make_shared<Stand>("Stand 1");
    const auto stand2 = std::make_shared<Stand>("Stand 2");

    stand1->addConstraint<Electricity>();
    stand2->addConstraint<Compatibility>();

    const auto compatibilityConstraint = stand2->getConstraint<Compatibility>().get();
    compatibilityConstraint->addIncompatibleStand(stand1);
    plan->addStand(stand1);
    plan->addStand(stand2);
    std::cout << *plan << std::endl;
    const auto spaceConstraint = stand2->getConstraint<Space>().get();
    std::cout << spaceConstraint->check(*plan);
    return 0;
}
