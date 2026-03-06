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
        if (auto c = stand->getConstraint<Electricity>(); c != nullptr)
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

QList<QObject*> Plan::getStandsForQml() {
    QList<QObject*> list;
    for(auto& s : _stands)
        list.append(s.get());
    return list;
}