/*
// Mangrove Project
// main.cpp
// Description: Qt application entry point
// Copyright (c) Mangrove. All rights reserved.
*/

#include <qqml.h>
#include <QQmlContext>
#include <QQmlApplicationEngine>

#include "Plan.hpp"
#include "DrawingCanvas/DrawingCanvasBackend.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    const auto plan = std::make_shared<Plan>();

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("planData", plan.get());
    qmlRegisterType<DrawingCanvasBackend>("Mangrove", 1, 0, "DrawingCanvasBackend");

    const QUrl url("../src/style/Main.qml");
    engine.load(url);

    return QGuiApplication::exec();
}