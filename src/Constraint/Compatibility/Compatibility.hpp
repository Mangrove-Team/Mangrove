/*
// Mangrove Project
// Compatibility.hpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_COMPATIBILITY_HPP
#define MANGROVE_COMPATIBILITY_HPP

#include <list>

#include "Stand.hpp"
#include "AConstraint.hpp"

class Compatibility : public AConstraint {
public:
    explicit Compatibility();

    [[nodiscard]] std::string getDataInfo() const override;
    [[nodiscard]] const std::list<std::shared_ptr<Stand>>& getIncompatibleStands() const;

    void addIncompatibleStand(const std::shared_ptr<Stand>& stand);
private:
    std::list<std::shared_ptr<Stand>> _incompatibleStands;
};

#endif //MANGROVE_COMPATIBILITY_HPP
