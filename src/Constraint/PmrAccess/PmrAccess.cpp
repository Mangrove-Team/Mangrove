/*
// Mangrove Project
// PmrAccess.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "PmrAccess.hpp"

namespace {
[[maybe_unused]] const bool pmrAccessRegistered = AConstraint::registerType([] {
    return std::make_shared<PmrAccess>();
});
}

PmrAccess::PmrAccess() {
    _name = "accès PMR";
}