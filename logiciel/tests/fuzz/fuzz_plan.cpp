/*
// Mangrove Project
// fuzz_plan.cpp
// Description: libFuzzer target. Interprets the input bytes as a sequence of
//              operations on a core::Plan (arbitrary doubles and ids included)
//              and aborts as soon as an invariant breaks. Run with ASan/UBSan.
//
//   ./fuzz_plan                       explore until a crash (Ctrl+C to stop)
//   ./fuzz_plan -max_total_time=60    one minute
//   ./fuzz_plan crash-<hash>          replay a finding
// Copyright (c) Mangrove. All rights reserved.
*/

#include "plan/Plan.h"

#include <fuzzer/FuzzedDataProvider.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <string>

using namespace mangrove::core;

namespace {

constexpr std::size_t kMaxStands = 64; // keeps each run fast

[[noreturn]] void broken(const char *what)
{
    std::fprintf(stderr, "invariant broken: %s\n", what);
    std::abort();
}

void checkInvariants(const Plan &plan)
{
    std::set<std::uint64_t> ids;
    for (std::size_t i = 0; i < plan.size(); ++i) {
        const Stand &stand = plan.stands()[i];
        if (!stand.id)
            broken("invalid id stored");
        if (!ids.insert(stand.id.value).second)
            broken("duplicate id");
        if (!stand.rect.isValid())
            broken("invalid rect stored");
        if (plan.indexOf(stand.id) != i)
            broken("indexOf disagrees with storage");
    }
}

// Raw doubles: NaN, infinities and denormals included.
RectM consumeRect(FuzzedDataProvider &data)
{
    return {data.ConsumeFloatingPoint<double>(), data.ConsumeFloatingPoint<double>(),
        data.ConsumeFloatingPoint<double>(), data.ConsumeFloatingPoint<double>()};
}

// Half the time an existing id, otherwise anything.
StandId consumeId(FuzzedDataProvider &data, const Plan &plan)
{
    if (plan.size() > 0 && data.ConsumeBool())
        return plan.stands()[data.ConsumeIntegralInRange<std::size_t>(0, plan.size() - 1)].id;
    return StandId{data.ConsumeIntegral<std::uint64_t>()};
}

} // namespace

// Each step is a regular creation (its generated label never collides with an
// existing one; foreign stands may share a label, no policy yet), a stand coming
// from outside (file, clipboard...) with arbitrary id and label, a removal, a rect
// update, a standAt query or a new hall.
extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t *bytes, std::size_t size)
{
    FuzzedDataProvider data(bytes, size);
    Plan plan;

    while (data.remaining_bytes() > 0) {
        switch (data.ConsumeIntegralInRange<int>(0, 5)) {
        case 0:
            if (plan.size() < kMaxStands) {
                const std::size_t at = data.ConsumeIntegral<std::size_t>();
                const Stand stand = plan.makeStand(consumeRect(data));
                for (const Stand &other : plan.stands()) {
                    if (stand.id && other.label == stand.label)
                        broken("makeStand produced a label already in use");
                }
                const bool expected = plan.canInsert(stand);
                if (plan.insertStand(at, stand) != expected)
                    broken("insertStand disagrees with canInsert");
            }
            break;
        case 1:
            if (plan.size() < kMaxStands) {
                const Stand foreign{
                    consumeId(data, plan), data.ConsumeRandomLengthString(8), consumeRect(data)};
                plan.insertStand(data.ConsumeIntegral<std::size_t>(), foreign);
            }
            break;
        case 2:
            plan.removeStand(consumeId(data, plan));
            break;
        case 3: {
            const StandId id = consumeId(data, plan);
            const RectM rect = consumeRect(data);
            if (plan.setStandRect(id, rect) && !(plan.find(id)->rect == rect))
                broken("setStandRect reported success but stored another rect");
            break;
        }
        case 4: {
            const Vec2 point{
                data.ConsumeFloatingPoint<double>(), data.ConsumeFloatingPoint<double>()};
            const StandId hit = plan.standAt(point);
            if (hit && !plan.find(hit)->rect.contains(point))
                broken("standAt returned a stand that does not contain the point");
            break;
        }
        case 5:
            plan.setHall(consumeRect(data));
            break;
        }
        checkInvariants(plan);
    }
    return 0;
}
