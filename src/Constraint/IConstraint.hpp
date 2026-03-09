/*
// Mangrove Project
// IConstraint.hpp
// Description:
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_ICONSTRAINT_H
#define MANGROVE_ICONSTRAINT_H

#include <string>
#include <QObject>
#include <typeindex>

class IConstraint {
public:
    virtual ~IConstraint() = default;

    [[nodiscard]] virtual std::string& getName() = 0;
    [[nodiscard]] virtual std::type_index getType() const = 0;
};

#endif //MANGROVE_ICONSTRAINT_H