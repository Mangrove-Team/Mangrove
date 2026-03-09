/*
// Mangrove Project
// main.cpp
// Description: Qt application entry point
// Copyright (c) Mangrove. All rights reserved.
*/

#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqml.h>
#include "DrawingCanvas/DrawingCanvasBackend.hpp"

#include "Plan.hpp"
#include "Electricity.hpp"
#include "IConstraint.hpp"

void handleStand1(const std::shared_ptr<Plan>& plan) {
    const auto stand = std::make_shared<Stand>();
    stand->setName(static_cast<std::string>("Stand 1"));
    stand->addConstraint<Electricity>(12.5);
    plan->addStand(stand);
}

void handleStand2(const std::shared_ptr<Plan>& plan) {
    const auto stand = std::make_shared<Stand>();
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
    qmlRegisterType<DrawingCanvasBackend>("Mangrove", 1, 0, "DrawingCanvasBackend");

    const QUrl url("../src/style/Main.qml");
    engine.load(url);

    return QGuiApplication::exec();
}