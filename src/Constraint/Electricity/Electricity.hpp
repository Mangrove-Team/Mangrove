/*
// Mangrove Project
// Electricity.hpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_ELECTRICITY_HPP
#define MANGROVE_ELECTRICITY_HPP

#include <ostream>

#include "AConstraint.hpp"

class Electricity : public AConstraint {
public:
    explicit Electricity(float watt = 10.0f);

    [[nodiscard]] float getWatt() const;
    [[nodiscard]] std::string getDataInfo() const override;
    
private:
    float _watt;
};

inline std::ostream& operator<<(std::ostream& os, const Electricity& constraint) {
    os << constraint.getName() << " (" << constraint.getWatt() << "W)";
    return os;
}

#endif //MANGROVE_ELECTRICITY_HPP
