/*
// Mangrove Project
// AConstraint.hpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_ACONSTRAINT_HPP
#define MANGROVE_ACONSTRAINT_HPP

#include "IConstraint.hpp"

class AConstraint : public Constraint::IConstraint {
public:
    AConstraint();
    ~AConstraint() override = default;

    std::string getName() const override;
    Constraint::Type getType() const override;
protected:
    std::string _name;
    Constraint::Type _type;
};

#endif //MANGROVE_ACONSTRAINT_HPP