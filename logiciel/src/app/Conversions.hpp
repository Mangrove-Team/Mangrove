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

    [[nodiscard]] inline QPointF toQt(core::Vec2 point)
    {
        return {point.x, point.y};
    }

    [[nodiscard]] inline QRectF toQt(const core::RectM &rect)
    {
        return {rect.x, rect.y, rect.width, rect.height};
    }

    [[nodiscard]] inline QString toQt(const std::string &text)
    {
        return QString::fromStdString(text);
    }

    [[nodiscard]] inline core::Vec2 toCore(QPointF point)
    {
        return {.x = point.x(), .y = point.y()};
    }

    [[nodiscard]] inline core::RectM toCore(const QRectF &rect)
    {
        return {.x = rect.x(), .y = rect.y(), .width = rect.width(), .height = rect.height()};
    }

} // namespace mangrove::app
