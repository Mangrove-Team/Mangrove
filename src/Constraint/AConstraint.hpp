/*
// Mangrove Project
// AConstraint.hpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_ACONSTRAINT_HPP
#define MANGROVE_ACONSTRAINT_HPP

#include <memory>
#include <functional>

#include "IConstraint.hpp"

class AConstraint : public IConstraint {
public:
    using Factory = std::function<std::shared_ptr<AConstraint>()>;
    struct RegisteredConstraint {
        QString id;
        Factory factory;
    };

    AConstraint();
    ~AConstraint() override = default;

    [[nodiscard]] std::string& getName() override;
    [[nodiscard]] std::type_index getType() const override;

    static bool registerType(const QString& id, Factory factory);
    static std::shared_ptr<AConstraint> createFromId(const QString& id);
    static QStringList registeredConstraintIds();
protected:
    std::string _name;
    std::type_index _type;
};

#endif //MANGROVE_ACONSTRAINT_HPP