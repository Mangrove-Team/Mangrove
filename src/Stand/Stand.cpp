/*
// Mangrove Project
// Stand.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Stand.hpp"
#include <QFile>

QString& Stand::getName() {
    return _name;
}

QStringList Stand::getConstraintsForQml() const {
    QStringList constraints;

    for (const auto &val: _constraints | std::views::values)
        constraints.append(QString::fromStdString(val->getName()));
    return constraints;
}

std::map<std::type_index, std::shared_ptr<AConstraint>>& Stand::getConstraints() {
    return _constraints;
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

void Stand::setX(const float x) {
    _x = x;
    emit xChanged();
}

void Stand::setY(const float y) {
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

bool Stand::hasImage() const {
    return QFile::exists("plans/" + _name + ".png");
}