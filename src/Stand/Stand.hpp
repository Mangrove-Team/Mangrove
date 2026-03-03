/*
// Mangrove Project
// Stand.hpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_STAND_HPP
#define MANGROVE_STAND_HPP

#include <map>

#include "AConstraint.hpp"

class Stand {
public:
    Stand() = default;

    void addConstraint(const AConstraint& c);

    std::string getName() const;
    AConstraint getConstraintByType(Constraint::Type type) const;

private:
    std::string _name;
    std::map<Constraint::Type, AConstraint> _constraints;
};


#endif //MANGROVE_STAND_HPP