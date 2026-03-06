/*
// Mangrove Project
// Stand.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Stand.hpp"
#include "PmrAccess.hpp"
#include "Electricity.hpp"

QString& Stand::getName() {
    return _name;
}

QStringList Stand::getConstraintsForQml() const {
    QStringList constraints;

    for (const auto &val: _constraints | std::views::values)
        constraints.append(QString::fromStdString(val->getName()));
    return constraints;
}

std::map<Constraint::Type, std::shared_ptr<AConstraint>>& Stand::getConstraints() {
    return _constraints;
}

std::shared_ptr<AConstraint> Stand::getConstraintByType(const Constraint::Type type) const {
    if (_constraints.contains(type))
        return _constraints.at(type);
    return nullptr;
}

void Stand::setName(const QString& name) {
    _name = name;
    emit nameChanged();
}

void Stand::setName(const std::string& name) {
    _name = QString::fromStdString(name);
    emit nameChanged();
}

bool Stand::addConstraintByType(const int type) {
    const auto constraintType = static_cast<Constraint::Type>(type);

    if (constraintType == Constraint::None || getConstraintByType(constraintType) != nullptr)
        return false;

    switch (constraintType) {
        case Constraint::Electricity:
            addConstraint<Electricity>(12.5f);
            return true;
        case Constraint::PMRAccess:
            addConstraint<PmrAccess>();
            return true;
        default:
            return false;
    }
}

void Stand::addNextConstraint() {
    if (addConstraintByType(Constraint::Electricity))
        return;
    addConstraintByType(Constraint::PMRAccess);
}

float Stand::getX() const {
    return _x;
}

float Stand::getY() const {
    return _y;
}

void Stand::setX(float x) {
    _x = x;
    emit xChanged();
}

void Stand::setY(float y) {
    _y = y;
    emit yChanged();
}

bool Stand::isOnMap() const {
    return _onMap;
}

void Stand::setOnMap(const bool onMap) {
    _onMap = onMap;
    emit onMapChanged();
}