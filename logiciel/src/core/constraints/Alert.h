/*
// Mangrove Project
// Alert.h
// Description: A constraint violation. Carries no user-facing text: the app
//              layer turns `ruleId` into a translated message.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include "plan/Ids.h"

#include <string>
#include <vector>

namespace mangrove::core {

struct Alert {
    std::string ruleId;
    std::vector<StandId> stands;

    friend bool operator==(const Alert &, const Alert &) = default;
};

} // namespace mangrove::core
