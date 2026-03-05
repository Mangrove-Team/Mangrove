/*
// Mangrove Project
// Plan.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Plan.hpp"
#include "Electricity.hpp"

Plan::Plan(QObject *parent) : QObject(parent) {}

float Plan::getTotalWatt() const {
    float watt = 0.0f;

    for (const auto& stand : _stands)
        if (auto c = stand->getConstraintByType(Constraint::Electricity); c != nullptr)
            watt += std::dynamic_pointer_cast<Electricity>(c)->getWatt();
    return watt;
}

void Plan::addStand(const std::shared_ptr<Stand>& stand) {
    _stands.append(stand);
    emit standAdded(stand);
    emit totalWattChanged();
}

Q_INVOKABLE QList<std::shared_ptr<Stand>> Plan::getStandsByConstraint(const Constraint::Type type) const {
    Q_INVOKABLE QList<std::shared_ptr<Stand>> stands;

    for (const auto& stand : _stands)
        if (auto constraints = stand->getConstraints(); constraints.contains(type))
            stands.push_back(stand);
    return stands;
}
