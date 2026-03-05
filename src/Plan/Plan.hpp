/*
// Mangrove Project
// Plan.hpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#ifndef MANGROVE_PLAN_HPP
#define MANGROVE_PLAN_HPP

#include <list>
#include <QFile>
#include <QBoxLayout>
#include <QPushButton>

#include "Stand.hpp"

class Plan {
public:
    explicit Plan(const std::shared_ptr<QWidget>& window);

    [[nodiscard]] std::list<std::shared_ptr<Stand>> getStandsByConstraint(Constraint::Type type) const;
    [[nodiscard]] float getTotalWatt() const;

    void addStand(const Stand& stand);

private:
    std::shared_ptr<QWidget> _standListWidget;
    std::shared_ptr<QBoxLayout> _standListLayout;
    std::unique_ptr<QWidget> _planWidget;
    std::shared_ptr<QPushButton> _addStandButton;
    std::list<std::shared_ptr<Stand>> _stands;
};

#endif //MANGROVE_PLAN_HPP