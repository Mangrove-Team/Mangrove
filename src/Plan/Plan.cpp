/*
// Mangrove Project
// Plan.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Plan.hpp"
#include "Electricity.hpp"

Plan::Plan(const std::shared_ptr<QWidget>& window) {
    _standListWidget = std::make_shared<QWidget>(window.get());
    _standListWidget->setGeometry(1400, 0, 520, 1080);
    _standListWidget->setObjectName("StandList");
    _standListLayout = std::make_shared<QVBoxLayout>(_standListWidget.get());
    _planWidget = std::make_unique<QWidget>(window.get());
    _planWidget->setGeometry(0, 0, 1400, 1080);
    _planWidget->setObjectName("Map");
    _addStandButton = std::make_shared<QPushButton>("add Stand");
    _standListLayout->addWidget(_addStandButton.get());
}

std::list<std::shared_ptr<Stand>> Plan::getStandsByConstraint(const Constraint::Type type) const {
    std::list<std::shared_ptr<Stand>> stands;

    for (const auto& stand : _stands)
        if (auto constraints = stand->getConstraints(); constraints.contains(type))
            stands.push_back(stand);
    return stands;
}

void Plan::addStand(const Stand& stand) {
    _stands.push_back(std::make_shared<Stand>(stand));
}

float Plan::getTotalWatt() const {
    float watt = 0.0f;

    for (const auto& stand : _stands)
        if (auto c = stand->getConstraintByType(Constraint::Electricity); c != nullptr)
            watt += std::dynamic_pointer_cast<Electricity>(c)->getWatt();
    return watt;
}