/*
// Mangrove Project
// main.cpp
// Description: Qt application entry point
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Translation.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml/qqmlextensionplugin.h>

Q_IMPORT_QML_PLUGIN(Mangrove_AppPlugin)
Q_IMPORT_QML_PLUGIN(Mangrove_ThemePlugin)
Q_IMPORT_QML_PLUGIN(Mangrove_ControlsPlugin)
Q_IMPORT_QML_PLUGIN(Mangrove_ShellPlugin)
Q_IMPORT_QML_PLUGIN(Mangrove_PanelsPlugin)

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    QGuiApplication::setApplicationName(QStringLiteral("Mangrove"));
    QGuiApplication::setOrganizationName(QStringLiteral("Mangrove"));
    QGuiApplication::setApplicationVersion(QStringLiteral(MANGROVE_VERSION));

    const auto translator = mangrove::app::loadTranslation();
    if (translator)
        QCoreApplication::installTranslator(translator.get());

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        [] { QCoreApplication::exit(EXIT_FAILURE); }, Qt::QueuedConnection);
    engine.loadFromModule("Mangrove.Main", "Main");

    return QGuiApplication::exec();
}
