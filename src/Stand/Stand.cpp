/*
// Mangrove Project
// Stand.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Stand.hpp"

QString Stand::getName() const {
    return _name;
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