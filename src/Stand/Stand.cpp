/*
// Mangrove Project
// Stand.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include <QFile>
#include <QVariantMap>

#include "Stand.hpp"

QString& Stand::getName() {
    return _name;
}

QStringList Stand::getConstraintsForQml() const {
    QStringList constraints;

    for (const auto &val: _constraints | std::views::values)
        constraints.append(QString::fromStdString(val->getName()));
    return constraints;
}

QVariantList Stand::getConstraintCardsForQml() const {
    QVariantList cards;
    cards.reserve(static_cast<qsizetype>(_constraints.size()));

    for (const auto& constraint : _constraints | std::views::values) {
        QVariantMap card;
        const QString name = QString::fromStdString(constraint->getName());

        card.insert("name", name);
        card.insert("fields", constraint->getFieldsForQml());

        cards.append(card);
    }
    return cards;
}

QStringList Stand::getAvailableConstraintsForQml() {
    return AConstraint::registeredConstraintIds();
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
    return QFile::exists("plans/" + _name + ".svg");
}

void Stand::addConstraintFromQml(const QString& type) {
    const std::shared_ptr<AConstraint> constraint = AConstraint::createFromId(type);
    if (!constraint)
        return;
    if (const auto [_, inserted] = _constraints.emplace(constraint->getType(), constraint); inserted)
        emit constraintsChanged();
}

bool Stand::setFieldFromQml(const QString& constraintName, const QString& fieldName, const QVariant& value) {
    for (const auto& constraint : _constraints | std::views::values) {
        if (const QString name = QString::fromStdString(constraint->getName()); name.compare(constraintName, Qt::CaseInsensitive) != 0)
            continue;
        if (!constraint->setFieldFromQml(fieldName, value))
            return false;
        emit constraintsChanged();
        return true;
    }
    return false;
}
