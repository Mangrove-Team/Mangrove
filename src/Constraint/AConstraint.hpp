/*
// Mangrove Project
// AConstraint.hpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_ACONSTRAINT_HPP
#define MANGROVE_ACONSTRAINT_HPP

#include <string>
#include <ostream>
#include <typeindex>

#include "IConstraint.hpp"

class AConstraint : public IConstraint {
public:
    explicit AConstraint();
    ~AConstraint() override = default;

    [[nodiscard]] const std::string& getName() const override;
    [[nodiscard]] std::type_index getType() const override;
    [[nodiscard]] std::string getDataInfo() const override;

    [[nodiscard]] std::string check() const override;

protected:
    std::string _name;
    std::type_index _type;
};

inline std::ostream& operator<<(std::ostream& os, const AConstraint& constraint) {
    os << constraint.getName() << " (" << constraint.getDataInfo() << ")";
    return os;
}

#endif //MANGROVE_ACONSTRAINT_HPP
