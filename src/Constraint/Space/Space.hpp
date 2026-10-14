/*
// Mangrove Project
// Space.hpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_SPACE_HPP
#define MANGROVE_SPACE_HPP

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

#include "Plan.hpp"
#include "AConstraint.hpp"

using Point = boost::geometry::model::d2::point_xy<int>;
using Polygon = boost::geometry::model::polygon<Point>;

class Space : public AConstraint {
public:
    explicit Space();
    ~Space() override = default;

    [[nodiscard]] std::string getDataInfo() const override;
    [[nodiscard]] const Polygon& getDelimitation() const;

    void addPoint(int x, int y);
    void changePoint(int index, int x, int y, const std::list<std::shared_ptr<Space>>& other);

    [[nodiscard]] std::string check(const Plan &plan) const;

private:
    Polygon _delimitation;
};

#endif //MANGROVE_SPACE_HPP
