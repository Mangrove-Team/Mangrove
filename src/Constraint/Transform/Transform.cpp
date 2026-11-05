/*
// Mangrove Project
// Transform.cpp
// Description: Transform constraint class that represents stand position and delimitation in a plan
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Transform.hpp"

Transform::Transform() {
    _name = "Transform";

    boost::geometry::append(_delimitation, Point(0, 0));
    boost::geometry::append(_delimitation, Point(100, 0));
    boost::geometry::append(_delimitation, Point(100, 100));
    boost::geometry::append(_delimitation, Point(0, 100));

    boost::geometry::correct(_delimitation);
}

std::string Transform::getDataInfo() const {
    std::string dataInfo;

    for (auto& point: _delimitation.outer())
        dataInfo += "(" + std::to_string(point.x()) + ", " + std::to_string(point.y()) + "), ";
    if (!dataInfo.empty()) {
        dataInfo.pop_back();
        dataInfo.pop_back();
    }
    return dataInfo;
}

const Polygon &Transform::getDelimitation() const {
    return _delimitation;
}

void Transform::addPoint(const int x, const int y) {
    boost::geometry::append(_delimitation, Point(x, y));
}

void Transform::changePoint(const int index, const int x, const int y, const std::list<std::shared_ptr<Transform>>& other) {
    if (index < 0 || index >= _delimitation.outer().size())
        throw std::out_of_range("Index out of range");
    _delimitation.outer()[index] = Point(x, y);
}

std::string Transform::check(const std::shared_ptr<Plan>& plan) const {
    std::string errors;
    const auto &stands = plan->getStands();

    const auto currentStand = std::ranges::find_if(stands, [this](const auto& stand) {
        return stand->template getConstraint<Transform>().get() == this;
    });

    for (const auto& stand: stands)
        if (*currentStand != stand && boost::geometry::intersects(_delimitation, stand->getConstraint<Transform>()->getDelimitation()))
            errors += "Error: Transform of '" + currentStand->get()->getName() + "' overlaps with '" + stand->getName() + "'\n";
    return errors;
}
