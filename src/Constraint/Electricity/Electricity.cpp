/*
// Mangrove Project
// Electricity.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include <QVariantMap>

#include "Electricity.hpp"

namespace {
[[maybe_unused]] const bool electricityRegistered = AConstraint::registerType([] {
    return std::make_shared<Electricity>();
});
}

Electricity::Electricity(const float watt) {
    _watt = watt;
    _name = "Electricité";
}

float Electricity::getWatt() const {
    return _watt;
}

QVariantList Electricity::getFieldsForQml() const {
    QVariantMap wattField;

    wattField.insert("name", "watt");
    wattField.insert("label", "Watt");
    wattField.insert("type", "number");
    wattField.insert("value", _watt);
    wattField.insert("min", 0.0);

    return {wattField};
}

bool Electricity::setFieldFromQml(const QString& fieldName, const QVariant& value) {
    _watt = value.toFloat();
    return true;
}