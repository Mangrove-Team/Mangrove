/*
// Mangrove Project
// AConstraint.hpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_ACONSTRAINT_HPP
#define MANGROVE_ACONSTRAINT_HPP

#include "IConstraint.hpp"

class AConstraint : public IConstraint {
public:
    AConstraint();
    ~AConstraint() override = default;

    [[nodiscard]] std::string& getName() override;
    [[nodiscard]] std::type_index getType() const override;
protected:
    std::string _name;
    std::type_index _type;
};

#endif //MANGROVE_ACONSTRAINT_HPP