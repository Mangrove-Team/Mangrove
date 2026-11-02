/*
// Mangrove Project
// TestSeed.h
// Description: Seed of the randomized tests. Fixed by default so failures are
//              reproducible; override with MANGROVE_TEST_SEED=<n> to explore.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include <QtDebug>
#include <QtGlobal>

#include <cstdint>

namespace mangrove::test {

inline std::uint32_t testSeed()
{
    bool parsed = false;
    const auto fromEnv = qEnvironmentVariable("MANGROVE_TEST_SEED").toUInt(&parsed);
    const std::uint32_t seed = parsed ? fromEnv : 20261003U;
    qInfo("random seed: %u (set MANGROVE_TEST_SEED to change it)", seed);
    return seed;
}

} // namespace mangrove::test
