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

QVariantList AConstraint::getFieldsForQml() const {
    return {};
}

bool AConstraint::setFieldFromQml(const QString& fieldName, const QVariant& value) {
    Q_UNUSED(fieldName)
    Q_UNUSED(value)
    return false;
}

bool AConstraint::registerType(Factory factory) {
    if (!factory)
        return false;
    const auto instance = factory();
    if (!instance)
        return false;
    const QString displayName = QString::fromStdString(instance->getName());
    const QString normalizedId = normalizeConstraintId(displayName);
    if (normalizedId.isEmpty())
        return false;
    constraintRegistry().insert(normalizedId, RegisteredConstraint{displayName, std::move(factory)});
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