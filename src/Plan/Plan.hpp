/*
// Mangrove Project
// Plan.hpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_PLAN_HPP
#define MANGROVE_PLAN_HPP

#include <list>
#include <memory>

#include "Stand.hpp"

class Plan {
public:
    Plan() = default;
    ~Plan() = default;

    [[nodiscard]] const std::list<std::shared_ptr<Stand>>& getStands() const;
    template<typename T>
    [[nodiscard]] std::list<std::shared_ptr<Stand>> getStandsByConstraint() const;
    template<typename T>
    [[nodiscard]] std::list<std::shared_ptr<AConstraint>> getConstraints() const;

    void addStand(const std::shared_ptr<Stand>& stand);

private:
    std::list<std::shared_ptr<Stand>> _stands;
};

template<typename T>
std::list<std::shared_ptr<Stand>> Plan::getStandsByConstraint() const {
    std::list<std::shared_ptr<Stand>> stands;
    const auto type = std::type_index(typeid(T));

    for (const auto& stand : _stands)
        if (stand->getConstraints().contains(type))
            stands.push_back(stand);
    return stands;
}

template<typename T>
std::list<std::shared_ptr<AConstraint>> Plan::getConstraints() const {
    std::list<std::shared_ptr<AConstraint>> constraints;

    const auto type = std::type_index(typeid(T));

    for (const auto& stand : _stands) {
        if (stand->getConstraints().contains(type)) {
            constraints.push_back(stand->getConstraints().at(type));
        }
    }
    return constraints;
}

inline std::ostream &operator<<(std::ostream &os, const Plan &plan) {
    bool first = true;

    for (const auto &stand: plan.getStands()) {
        if (!first)
            os << "\n";
        first = false;
        os << *stand;
    }
    return os;
}

#endif //MANGROVE_PLAN_HPP
