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
    _standsForQml.append(stand.get());
    connect(stand.get(), &Stand::constraintsChanged, this, [this]() {
        emit totalWattChanged();
    });

    emit standAdded(stand);
    emit standsChanged();
    emit totalWattChanged();
}

QList<QObject*> Plan::getStandsForQml() {
    return _standsForQml;
}

void Plan::addStandFromQml(const QString& name) {
    const auto stand = std::make_shared<Stand>();
    stand->setName(name);
    addStand(stand);
}

void Plan::notifyStandImageChanged(const QString& standName) {
    for (const auto& stand : _stands) {
        if (stand->getName() == standName) {
            emit stand->hasImageChanged();
            break;
        }
    }
}
