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

    friend constexpr Vec2 operator+(Vec2 a, Vec2 b) noexcept
    {
        return {a.x + b.x, a.y + b.y};
    }
    
    friend constexpr Vec2 operator-(Vec2 a, Vec2 b) noexcept
    {
        return {a.x - b.x, a.y - b.y};
    }
    
    friend constexpr Vec2 operator*(Vec2 v, double k) noexcept
    {
        return {v.x * k, v.y * k};
    }
    
    friend constexpr bool operator==(Vec2, Vec2) noexcept = default;
};

struct RectM {
    double x = 0.0;
    double y = 0.0;
    double width = 0.0;
    double height = 0.0;

    [[nodiscard]] constexpr double left() const noexcept
    {
        return x;
    }
    
    [[nodiscard]] constexpr double top() const noexcept
    {
        return y;
    }
    
    [[nodiscard]] constexpr double right() const noexcept
    {
        return x + width;
    }
    
    [[nodiscard]] constexpr double bottom() const noexcept
    {
        return y + height;
    }
    
    [[nodiscard]] constexpr Vec2 topLeft() const noexcept
    {
        return {x, y};
    }
    
    [[nodiscard]] constexpr Vec2 size() const noexcept
    {
        return {width, height};
    }
    
    [[nodiscard]] constexpr bool isEmpty() const noexcept
    {
        return width <= 0.0 || height <= 0.0;
    }

    [[nodiscard]] bool isValid() const noexcept
    {
        return std::isfinite(x) && std::isfinite(y) && std::isfinite(right()) && std::isfinite(bottom()) && width >= 0.0 && height >= 0.0;
    }

    [[nodiscard]] constexpr bool contains(Vec2 p) const noexcept
    {
        return p.x >= left() && p.x <= right() && p.y >= top() && p.y <= bottom();
    }

    [[nodiscard]] constexpr bool contains(const RectM &other) const noexcept
    {
        return other.left() >= left() && other.right() <= right() && other.top() >= top() && other.bottom() <= bottom();
    }

    [[nodiscard]] constexpr bool intersects(const RectM &other) const noexcept
    {
        return left() < other.right() && other.left() < right() && top() < other.bottom() && other.top() < bottom();
    }

    [[nodiscard]] constexpr RectM translated(Vec2 delta) const noexcept
    {
        return {x + delta.x, y + delta.y, width, height};
    }

    [[nodiscard]] constexpr RectM movedTo(Vec2 topLeft) const noexcept
    {
        return {topLeft.x, topLeft.y, width, height};
    }

    [[nodiscard]] static constexpr RectM fromCorners(Vec2 a, Vec2 b) noexcept
    {
        const double minX = std::min(a.x, b.x);
        const double minY = std::min(a.y, b.y);
        return {minX, minY, std::max(a.x, b.x) - minX, std::max(a.y, b.y) - minY};
    }

    friend constexpr bool operator==(const RectM &, const RectM &) noexcept = default;
};

} // namespace mangrove::core
