/*
// Mangrove Project
// Plan.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Plan.hpp"
#include "Electricity.hpp"

Plan::Plan(QObject *parent)
    : QObject(parent)
{
}

float Plan::getTotalWatt() const {
    float watt = 0.0f;

    for (const auto& stand : _stands)
        if (auto c = stand->getConstraintByType(Constraint::Electricity); c != nullptr)
            watt += std::dynamic_pointer_cast<Electricity>(c)->getWatt();
    return watt;
}

void Plan::addStand(const std::shared_ptr<Stand>& stand) {
    _stands.append(stand);

    connect(stand.get(), &Stand::constraintsChanged, this, [this]() {
        emit totalWattChanged();
    });

    emit standAdded(stand);
    emit standsChanged();
    emit totalWattChanged();
}

QList<std::shared_ptr<Stand>> Plan::getStandsByConstraint(const Constraint::Type type) const {
    QList<std::shared_ptr<Stand>> stands;

    for (const auto& stand : _stands)
        if (auto& constraints = stand->getConstraints(); constraints.contains(type))
            stands.push_back(stand);
    return stands;
}

QList<QObject*> Plan::getStandsForQml() {
    QList<QObject*> list;
    for(auto& s : _stands)
        list.append(s.get());
    return list;
}