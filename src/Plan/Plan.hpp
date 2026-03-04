/*
// Mangrove Project
// Plan.hpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_PLAN_HPP
#define MANGROVE_PLAN_HPP

#include <list>

#include "Stand.hpp"

class Plan {
public:
    Plan() = default;

    [[nodiscard]] std::list<std::shared_ptr<Stand>> getStandsByConstraint(Constraint::Type type) const;
    [[nodiscard]] float getTotalWatt() const;

    void addStand(const Stand& stand);

private:
    std::list<std::shared_ptr<Stand>> _stands;
};

#endif //MANGROVE_PLAN_HPP