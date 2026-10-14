/*
// Mangrove Project
// Space.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include <memory>

#include "Space.hpp"

Space::Space() {
    _name = "Space";

    boost::geometry::append(_delimitation, Point(0, 0));
    boost::geometry::append(_delimitation, Point(100, 0));
    boost::geometry::append(_delimitation, Point(100, 100));
    boost::geometry::append(_delimitation, Point(0, 100));

    boost::geometry::correct(_delimitation);
}

std::string Space::getDataInfo() const {
    std::string dataInfo;

    for (auto& point: _delimitation.outer()) {
        dataInfo += "(" + std::to_string(point.x()) + ", " + std::to_string(point.y()) + "), ";
    }
    if (!dataInfo.empty()) {
        dataInfo.pop_back();
        dataInfo.pop_back();
    }
    return dataInfo;
}

const Polygon &Space::getDelimitation() const {
    return _delimitation;
}

void Space::addPoint(const int x, const int y) {
    boost::geometry::append(_delimitation, Point(x, y));
}

void Space::changePoint(const int index, const int x, const int y, const std::list<std::shared_ptr<Space>>& other) {
    if (index < 0 || index >= _delimitation.outer().size())
        throw std::out_of_range("Index out of range");
    _delimitation.outer()[index] = Point(x, y);
}

std::string Space::check(const Plan &plan) const {
    std::string errors;
    const auto &stands = plan.getStands();

    const auto currentStand = std::ranges::find_if(stands, [this](const auto& stand) {
        return stand->template getConstraint<Space>().get() == this;
    });

    for (const auto& stand: stands) {
        if (*currentStand != stand && boost::geometry::intersects(_delimitation, stand->getConstraint<Space>()->getDelimitation()))
            errors += "Error: Space of '" + currentStand->get()->getName() + "' overlaps with '" + stand->getName() + "'\n";
    }
    return errors;
}
