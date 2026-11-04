/*
// Mangrove Project
// tst_plan_invariants.cpp
// Description: Applies thousands of random operations to a Plan and checks,
//              after each one, the invariants every other layer relies on.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "TestSeed.h"

#include "geometry/Geometry.h"
#include "plan/Ids.h"
#include "plan/Plan.h"
#include "plan/Stand.h"

#include <QtTest>

#include <array>
#include <cstddef>
#include <cstdint>
#include <random>
#include <set>
#include <string>

using namespace mangrove::core;

namespace {

// Returns an empty string when standAt(point) is the topmost stand containing the
// point (or no stand when none contains it), else what broke.
QString brokenStandAt(const Plan &plan, Vec2 point)
{
    const StandId hit = plan.standAt(point);
    for (std::size_t i = plan.size(); i-- > 0;) {
        if (plan.stands()[i].rect.contains(point)) {
            return hit == plan.stands()[i].id ? QString()
                                              : QStringLiteral("standAt missed the topmost stand");
        }
    }
    return hit ? QStringLiteral("standAt found a stand where there is none") : QString();
}

// Returns an empty string when every invariant holds, else what broke.
QString brokenInvariant(const Plan &plan)
{
    std::set<std::uint64_t> ids;
    std::set<std::string> labels;
    for (std::size_t i = 0; i < plan.size(); ++i) {
        const Stand &stand = plan.stands()[i];
        if (!stand.id) {
            return QStringLiteral("stand %1 has an invalid id").arg(i);
        }
        if (!ids.insert(stand.id.value).second) {
            return QStringLiteral("duplicate id %1").arg(stand.id.value);
        }
        if (!labels.insert(stand.label).second) {
            return QStringLiteral("duplicate label %1").arg(QString::fromStdString(stand.label));
        }
        if (!stand.rect.isValid()) {
            return QStringLiteral("stand %1 has an invalid rect").arg(i);
        }
        if (plan.indexOf(stand.id) != i || plan.find(stand.id) != &stand) {
            return QStringLiteral("indexOf/find disagree for stand %1").arg(i);
        }
    }
    return {};
}

} // namespace

constexpr std::size_t kMaxStands = 150;
constexpr int kSteps = 5000;
// Coordinates span the default 60 x 40 m hall plus a margin outside it.
constexpr double kMinCoord = -10.0;
constexpr double kMaxCoord = 70.0;
constexpr double kMaxStandSide = 8.0;

enum class Operation : std::uint8_t {
    Insert,
    Remove,
    Resize,
    ReinsertExisting,
    QueryStandAt,
};

// Drawn uniformly: an operation listed twice is twice as likely.
constexpr std::array kOperations{Operation::Insert, Operation::Insert, Operation::Remove,
    Operation::Resize, Operation::ReinsertExisting, Operation::QueryStandAt};

class TestPlanInvariants : public QObject {
    Q_OBJECT

private slots:
    // Random inserts anywhere in the stack, removals, rect updates, re-insertions of
    // existing ids (which must be refused) and standAt queries (which must return the
    // topmost stand containing the point). Once the plan reaches kMaxStands, inserts
    // become removals to keep it small. Invariants are checked after each step.
    void randomOperationsKeepInvariants()
    {
        std::mt19937 rng(mangrove::test::testSeed());
        std::uniform_int_distribution<std::size_t> pickOperation(0, kOperations.size() - 1);
        std::uniform_real_distribution<double> coord(kMinCoord, kMaxCoord);
        std::uniform_real_distribution<double> size(0.0, kMaxStandSide);
        const auto randomRect = [&] {
            return RectM{.x = coord(rng), .y = coord(rng), .width = size(rng), .height = size(rng)};
        };
        const auto randomStand = [&](const Plan &plan) {
            std::uniform_int_distribution<std::size_t> pick(0, plan.size() - 1);
            return plan.stands()[pick(rng)].id;
        };

        Plan plan;
        for (int step = 0; step < kSteps; ++step) {
            Operation next = kOperations.at(pickOperation(rng));
            if (plan.size() >= kMaxStands && next == Operation::Insert) {
                next = Operation::Remove;
            }
            switch (next) {
            case Operation::Insert: {
                std::uniform_int_distribution<std::size_t> position(0, plan.size());
                QVERIFY(plan.insertStand(position(rng), plan.makeStand(randomRect())));
                break;
            }
            case Operation::Remove:
                if (plan.size() > 0) {
                    QVERIFY(plan.removeStand(randomStand(plan)).has_value());
                }
                break;
            case Operation::Resize:
                if (plan.size() > 0) {
                    QVERIFY(plan.setStandRect(randomStand(plan), randomRect()));
                }
                break;
            case Operation::ReinsertExisting: {
                if (plan.size() > 0) {
                    const Stand copy = *plan.find(randomStand(plan));
                    QVERIFY(!plan.insertStand(0, copy));
                }
                break;
            }
            case Operation::QueryStandAt: {
                const QString broken = brokenStandAt(plan, {.x = coord(rng), .y = coord(rng)});
                QVERIFY2(broken.isEmpty(), qPrintable(broken));
                break;
            }
            }

            const QString broken = brokenInvariant(plan);
            if (!broken.isEmpty()) {
                QFAIL(qPrintable(QStringLiteral("step %1: %2").arg(step).arg(broken)));
            }
        }
    }
};

QTEST_APPLESS_MAIN(TestPlanInvariants)
#include "tst_plan_invariants.moc"
