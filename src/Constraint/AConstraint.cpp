/*
// Mangrove Project
// AConstraint.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "AConstraint.hpp"

#include <QHash>

namespace {
QHash<QString, AConstraint::RegisteredConstraint>& constraintRegistry() {
    static QHash<QString, AConstraint::RegisteredConstraint> registry;
    return registry;
}

QString normalizeConstraintId(const QString& id) {
    return id.trimmed().toLower();
}
}

AConstraint::AConstraint(): _type(std::type_index(typeid(*this))){}

std::string& AConstraint::getName() {
    return _name;
}

std::type_index AConstraint::getType() const {
    return typeid(*this);
}

bool AConstraint::registerType(const QString& id, Factory factory) {
    const QString normalizedId = normalizeConstraintId(id);
    const QString displayId = id.trimmed();
    if (normalizedId.isEmpty() || displayId.isEmpty() || !factory)
        return false;
    constraintRegistry().insert(normalizedId, RegisteredConstraint{displayId, std::move(factory)});
    return true;
}

std::shared_ptr<AConstraint> AConstraint::createFromId(const QString& id) {
    const QString normalizedId = normalizeConstraintId(id);
    if (normalizedId.isEmpty())
        return nullptr;
    const auto entryIt = constraintRegistry().constFind(normalizedId);
    if (entryIt == constraintRegistry().cend())
        return nullptr;
    return entryIt->factory();
}

QStringList AConstraint::registeredConstraintIds() {
    QStringList ids;
    ids.reserve(constraintRegistry().size());
    for (const auto&[id, factory] : constraintRegistry())
        ids.append(id);
    ids.sort(Qt::CaseInsensitive);
    return ids;
}