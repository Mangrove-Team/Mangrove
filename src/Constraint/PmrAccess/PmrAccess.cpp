/*
// Mangrove Project
// PmrAccess.cpp
// Description: 
// Copyright (c) Mangrove. All rights reserved.
*/

#include "PmrAccess.hpp"

namespace {
[[maybe_unused]] const bool pmrAccessRegistered = AConstraint::registerType("PMR access", [] {
    return std::make_shared<PmrAccess>();
});
}

PmrAccess::PmrAccess() {
    _name = "PMR access";
}