/*
// Mangrove Project
// Translation.cpp
// Description: Loads the translation of the interface.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Translation.hpp"

#include <QString>

namespace mangrove::app {

    std::unique_ptr<QTranslator> loadTranslation()
    {
        auto translator = std::make_unique<QTranslator>();
        if (!translator->load(QStringLiteral(":/i18n/mangrove_fr.qm")))
            return nullptr;
        return translator;
    }

} // namespace mangrove::app
