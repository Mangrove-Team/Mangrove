/*
// Mangrove Project
// Electricity.h
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_ELECTRICITY_H
#define MANGROVE_ELECTRICITY_H

#include "AConstraint.hpp"

class Electricity : public AConstraint {
public:
    explicit Electricity(float watt = 10.0f);

    [[nodiscard]] float getWatt() const;
    [[nodiscard]] QVariantList getFieldsForQml() const override;
    bool setFieldFromQml(const QString& fieldName, const QVariant& value) override;
private:
    float _watt;
};

#endif //MANGROVE_ELECTRICITY_H