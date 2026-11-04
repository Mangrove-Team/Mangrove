/*
// Mangrove Project
// Geometry.h
// Description: Plane geometry primitives. Every length is expressed in meters.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include <algorithm>
#include <cmath>

namespace mangrove::core {

struct Vec2 {
    double x = 0.0;
    double y = 0.0;

    friend constexpr Vec2 operator+(Vec2 lhs, Vec2 rhs) noexcept
    {
        return {.x = lhs.x + rhs.x, .y = lhs.y + rhs.y};
    }

    friend constexpr Vec2 operator-(Vec2 lhs, Vec2 rhs) noexcept
    {
        return {.x = lhs.x - rhs.x, .y = lhs.y - rhs.y};
    }

    friend constexpr Vec2 operator*(Vec2 vec, double factor) noexcept
    {
        return {.x = vec.x * factor, .y = vec.y * factor};
    }

    friend constexpr bool operator==(Vec2, Vec2) noexcept = default;
};

struct RectM {
    double x = 0.0;
    double y = 0.0;
    double width = 0.0;
    double height = 0.0;

    [[nodiscard]] constexpr double left() const noexcept { return x; }

    [[nodiscard]] constexpr double top() const noexcept { return y; }

    [[nodiscard]] constexpr double right() const noexcept { return x + width; }

    [[nodiscard]] constexpr double bottom() const noexcept { return y + height; }

    [[nodiscard]] constexpr Vec2 topLeft() const noexcept { return {.x = x, .y = y}; }

    [[nodiscard]] constexpr Vec2 size() const noexcept { return {.x = width, .y = height}; }

    [[nodiscard]] constexpr bool isEmpty() const noexcept { return width <= 0.0 || height <= 0.0; }

    [[nodiscard]] bool isValid() const noexcept
    {
        return std::isfinite(x) && std::isfinite(y) && std::isfinite(right())
            && std::isfinite(bottom()) && width >= 0.0 && height >= 0.0;
    }

    [[nodiscard]] constexpr bool contains(Vec2 point) const noexcept
    {
        return point.x >= left() && point.x <= right() && point.y >= top() && point.y <= bottom();
    }

    [[nodiscard]] constexpr bool contains(const RectM &other) const noexcept
    {
        return other.left() >= left() && other.right() <= right() && other.top() >= top()
            && other.bottom() <= bottom();
    }

    [[nodiscard]] constexpr bool intersects(const RectM &other) const noexcept
    {
        return left() < other.right() && other.left() < right() && top() < other.bottom()
            && other.top() < bottom();
    }

    [[nodiscard]] constexpr RectM translated(Vec2 delta) const noexcept
    {
        return {.x = x + delta.x, .y = y + delta.y, .width = width, .height = height};
    }

    [[nodiscard]] constexpr RectM movedTo(Vec2 topLeft) const noexcept
    {
        return {.x = topLeft.x, .y = topLeft.y, .width = width, .height = height};
    }

    [[nodiscard]] static constexpr RectM fromCorners(Vec2 corner, Vec2 opposite) noexcept
    {
        const double minX = std::min(corner.x, opposite.x);
        const double minY = std::min(corner.y, opposite.y);
        return {.x = minX,
            .y = minY,
            .width = std::max(corner.x, opposite.x) - minX,
            .height = std::max(corner.y, opposite.y) - minY};
    }

    friend constexpr bool operator==(const RectM &, const RectM &) noexcept = default;
};

} // namespace mangrove::core
