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

namespace Constraint {
    Q_NAMESPACE

    enum Type {
        None = 0,
        Electricity,
        PMRAccess,
        Luminosity,
    };
    Q_ENUM_NS(Type)

    class IConstraint {
    public:
        virtual ~IConstraint() = default;

        [[nodiscard]] virtual std::string& getName() = 0;
        [[nodiscard]] virtual Type getType() const = 0;
    };
}

#endif //MANGROVE_ICONSTRAINT_H