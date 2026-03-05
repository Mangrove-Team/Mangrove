/*
// Mangrove Project
// main.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Plan.hpp"
#include "Electricity.hpp"

void handleStand1(std::shared_ptr<Plan> plan) {
    auto stand = std::make_shared<Stand>();
    stand->setName(static_cast<std::string>("Stand 1"));
    stand->addConstraint<Electricity>(12.5);
    plan->addStand(stand);
}

void handleStand2(std::shared_ptr<Plan> plan) {
    auto stand = std::make_shared<Stand>();
    stand->setName(static_cast<std::string>("Stand 2"));
    plan->addStand(stand);
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    const auto plan = std::make_shared<Plan>();
    handleStand1(plan);
    handleStand2(plan);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("planData", plan.get());

    const QUrl url("../src/style/Main.qml");
    engine.load(url);

    return QGuiApplication::exec();
}