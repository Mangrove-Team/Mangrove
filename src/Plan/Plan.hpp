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

    std::list<Stand> getStandsByConstraint(Constraint::Type type) const;

private:
    std::list<Stand> _stands;
};


#endif //MANGROVE_PLAN_HPP