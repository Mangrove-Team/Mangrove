/*
// Mangrove Project
// Translation.hpp
// Description: Loads the translation of the interface. Texts are written in English in the code and shown in French.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include <QTranslator>

#include <memory>

namespace mangrove::app {

    // The French translation embedded in the app, or null if it cannot be loaded.
    // Install it with QCoreApplication::installTranslator() before loading any QML;
    // it uninstalls itself when destroyed.
    [[nodiscard]] std::unique_ptr<QTranslator> loadTranslation();

} // namespace mangrove::app
