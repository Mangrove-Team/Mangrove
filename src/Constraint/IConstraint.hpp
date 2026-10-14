/*
// Mangrove Project
// IConstraint.hpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_ICONSTRAINT_HPP
#define MANGROVE_ICONSTRAINT_HPP

#include <string>
#include <typeindex>

class IConstraint {
public:
    virtual ~IConstraint() = default;

    [[nodiscard]] virtual const std::string& getName() const = 0;
    [[nodiscard]] virtual std::type_index getType() const = 0;
    [[nodiscard]] virtual std::string getDataInfo() const = 0;

    [[nodiscard]] virtual std::string check() const = 0;
};

#endif //MANGROVE_ICONSTRAINT_HPP
