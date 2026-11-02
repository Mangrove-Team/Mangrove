/*
// Mangrove Project
// Conversions.hpp
// Description: Value conversions between core types and Qt types. Units are untouched: meters stay meters.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include "geometry/Geometry.h"

#include <QPointF>
#include <QRectF>
#include <QString>

#include <string>

namespace mangrove::app {

    [[nodiscard]] inline QPointF toQt(core::Vec2 v)
    {
        return {v.x, v.y};
    }

    [[nodiscard]] inline QRectF toQt(const core::RectM &r)
    {
        return {r.x, r.y, r.width, r.height};
    }

    [[nodiscard]] inline QString toQt(const std::string &s)
    {
        return QString::fromStdString(s);
    }

    [[nodiscard]] inline core::Vec2 toCore(QPointF p)
    {
        return {p.x(), p.y()};
    }

    [[nodiscard]] inline core::RectM toCore(const QRectF &r)
    {
        return {r.x(), r.y(), r.width(), r.height()};
    }

} // namespace mangrove::app
