/*
// Mangrove Project
// main.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include <iostream>

#include "Plan.hpp"
#include "Stand.hpp"
#include "PmrAccess.hpp"
#include "Electricity.hpp"

auto handleStand1(const std::shared_ptr<Plan>& plan) -> void {
    Stand stand;

    stand.setName("stand 1");
    stand.addConstraint<Electricity>(12.5);
    stand.addConstraint<PmrAccess>();
    plan->addStand(stand);
}

auto handleStand2(const std::shared_ptr<Plan>& plan) -> void {
    Stand stand;

    stand.setName("stand 2");
    stand.addConstraint<Electricity>(5.3);
    plan->addStand(stand);
}

auto handleStand3(const std::shared_ptr<Plan>& plan) -> void {
    Stand stand;

    stand.setName("stand 3");
    stand.addConstraint<PmrAccess>();
    plan->addStand(stand);
}

auto main() -> int {
    const auto plan = std::make_shared<Plan>();

    handleStand1(plan);
    handleStand2(plan);
    handleStand3(plan);

    for (const auto standWithElectricity = plan->getStandsByConstraint(Constraint::Electricity); const auto& elt : standWithElectricity)
        std::cout << elt->getName() << std::endl;
}
