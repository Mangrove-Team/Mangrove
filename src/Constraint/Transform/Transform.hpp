/*
// Mangrove Project
// Transform.hpp
// Description: Transform constraint class that represents stand position and delimitation in a plan
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_TRANSFORM_HPP
#define MANGROVE_TRANSFORM_HPP

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

#include "Plan.hpp"
#include "AConstraint.hpp"

using Point = boost::geometry::model::d2::point_xy<int>;
using Polygon = boost::geometry::model::polygon<Point>;

class Transform : public AConstraint {
public:
    explicit Transform();
    ~Transform() override = default;

    [[nodiscard]] std::string getDataInfo() const override;
    [[nodiscard]] const Polygon& getDelimitation() const;

    void addPoint(int x, int y);
    void changePoint(int index, int x, int y, const std::list<std::shared_ptr<Transform>>& other);

    [[nodiscard]] std::string check(const std::shared_ptr<Plan>& plan) const;

private:
    Polygon _delimitation;
};

#endif //MANGROVE_TRANSFORM_HPP
