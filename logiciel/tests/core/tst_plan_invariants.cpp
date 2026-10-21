/*
// Mangrove Project
// tst_plan_invariants.cpp
// Description: Applies thousands of random operations to a Plan and checks,
//              after each one, the invariants every other layer relies on.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "TestSeed.h"

#include "plan/Plan.h"

#include <QtTest>

#include <random>
#include <set>
#include <string>

using namespace mangrove::core;

namespace {

// Returns an empty string when every invariant holds, else what broke.
QString brokenInvariant(const Plan &plan)
{
    std::set<std::uint64_t> ids;
    std::set<std::string> labels;
    for (std::size_t i = 0; i < plan.size(); ++i) {
        const Stand &stand = plan.stands()[i];
        if (!stand.id)
            return QStringLiteral("stand %1 has an invalid id").arg(i);
        if (!ids.insert(stand.id.value).second)
            return QStringLiteral("duplicate id %1").arg(stand.id.value);
        if (!labels.insert(stand.label).second)
            return QStringLiteral("duplicate label %1").arg(QString::fromStdString(stand.label));
        if (!stand.rect.isValid())
            return QStringLiteral("stand %1 has an invalid rect").arg(i);
        if (plan.indexOf(stand.id) != i || plan.find(stand.id) != &stand)
            return QStringLiteral("indexOf/find disagree for stand %1").arg(i);
    }
    return {};
}

} // namespace

constexpr std::size_t kMaxStands = 150;

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
        std::uniform_int_distribution<int> op(0, 5);
        std::uniform_real_distribution<double> coord(-10.0, 70.0);
        std::uniform_real_distribution<double> size(0.0, 8.0);
        const auto randomRect = [&] { return RectM{coord(rng), coord(rng), size(rng), size(rng)}; };
        const auto randomStand = [&](const Plan &plan) {
            std::uniform_int_distribution<std::size_t> pick(0, plan.size() - 1);
            return plan.stands()[pick(rng)].id;
        };

        Plan plan;
        for (int step = 0; step < 5000; ++step) {
            int next = op(rng);
            if (plan.size() >= kMaxStands && next <= 1)
                next = 2;
            switch (next) {
            case 0:
            case 1: {
                std::uniform_int_distribution<std::size_t> at(0, plan.size());
                QVERIFY(plan.insertStand(at(rng), plan.makeStand(randomRect())));
                break;
            }
            case 2:
                if (plan.size() > 0)
                    QVERIFY(plan.removeStand(randomStand(plan)).has_value());
                break;
            case 3:
                if (plan.size() > 0)
                    QVERIFY(plan.setStandRect(randomStand(plan), randomRect()));
                break;
            case 4: {
                if (plan.size() > 0) {
                    const Stand copy = *plan.find(randomStand(plan));
                    QVERIFY(!plan.insertStand(0, copy));
                }
                break;
            }
            case 5: {
                const Vec2 point{coord(rng), coord(rng)};
                const StandId hit = plan.standAt(point);
                for (std::size_t i = plan.size(); i-- > 0;) {
                    if (plan.stands()[i].rect.contains(point)) {
                        QCOMPARE(hit, plan.stands()[i].id);
                        break;
                    }
                }
                if (hit)
                    QVERIFY(plan.find(hit)->rect.contains(point));
                break;
            }
            }

            const QString broken = brokenInvariant(plan);
            if (!broken.isEmpty())
                QFAIL(qPrintable(QStringLiteral("step %1: %2").arg(step).arg(broken)));
        }
    }
};

QTEST_APPLESS_MAIN(TestPlanInvariants)
#include "tst_plan_invariants.moc"
