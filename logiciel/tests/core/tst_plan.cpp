/*
// Mangrove Project
// tst_plan.cpp
// Description: Tests of mangrove::core::Plan.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "geometry/Geometry.h"
#include "plan/Plan.h"
#include "plan/Stand.h"

#include <QtTest>

#include <cstddef>
#include <cstdint>
#include <optional>
#include <set>
#include <string>

using namespace mangrove::core;

class TestPlan : public QObject {
    Q_OBJECT

private slots:
    void defaultHall()
    {
        const Plan plan;
        QCOMPARE(plan.hall(), RectM(0, 0, 60, 40));
        QCOMPARE(plan.size(), std::size_t{0});
    }

    void makeStandGivesUniqueIdsAndLabels()
    {
        Plan plan;
        std::set<std::uint64_t> ids;
        std::set<std::string> labels;
        constexpr int kStands = 12;
        for (int i = 0; i < kStands; ++i) {
            const Stand stand = plan.makeStand({.x = 0, .y = 0, .width = 4, .height = 3});
            QVERIFY(static_cast<bool>(stand.id));
            ids.insert(stand.id.value);
            labels.insert(stand.label);
        }
        QCOMPARE(ids.size(), std::size_t{kStands});
        QCOMPARE(labels.size(), std::size_t{kStands});
        QVERIFY(labels.contains("S01"));
        QVERIFY(labels.contains("S12"));
    }

    void makeStandDoesNotInsert()
    {
        Plan plan;
        const Stand stand = plan.makeStand({.x = 0, .y = 0, .width = 4, .height = 3});
        QCOMPARE(plan.size(), std::size_t{0});
        QVERIFY(plan.find(stand.id) == nullptr);
    }

    // Insertion at an index keeps the stack order; a duplicate or invalid id is refused.
    void insertAndRemove()
    {
        Plan plan;
        const Stand first = plan.makeStand({.x = 0, .y = 0, .width = 4, .height = 3});
        const Stand second = plan.makeStand({.x = 10, .y = 0, .width = 4, .height = 3});
        QVERIFY(plan.insertStand(plan.size(), first));
        QVERIFY(plan.insertStand(0, second));
        QVERIFY(!plan.insertStand(0, first));
        QVERIFY(!plan.insertStand(0, Stand{}));

        QCOMPARE(plan.indexOf(second.id), std::optional<std::size_t>(0));
        QCOMPARE(plan.indexOf(first.id), std::optional<std::size_t>(1));

        const auto removed = plan.removeStand(second.id);
        QVERIFY(removed.has_value());
        QCOMPARE(removed, std::optional<Stand>(second));
        QCOMPARE(plan.size(), std::size_t{1});
        QVERIFY(plan.find(second.id) == nullptr);
        QVERIFY(!plan.removeStand(second.id).has_value());
    }

    void reinsertedIdIsNeverReallocated()
    {
        Plan source;
        const Stand far = Stand{
            .id = StandId{42}, .label = "S42", .rect = {.x = 0, .y = 0, .width = 4, .height = 3}};
        QVERIFY(source.insertStand(0, far));
        QVERIFY(source.makeStand({0, 0, 1, 1}).id.value > 42);
    }

    void setStandRect()
    {
        Plan plan;
        const Stand first = plan.makeStand({.x = 0, .y = 0, .width = 4, .height = 3});
        plan.insertStand(0, first);
        QVERIFY(plan.setStandRect(first.id, {5, 5, 4, 3}));
        QCOMPARE(plan.find(first.id)->rect, RectM(5, 5, 4, 3));
        QVERIFY(!plan.setStandRect(StandId{999}, {0, 0, 1, 1}));
    }

    // Where several stands cover a point, standAt returns the topmost one.
    void standAtReturnsTopmost()
    {
        Plan plan;
        const Stand bottom = plan.makeStand({.x = 0, .y = 0, .width = 4, .height = 3});
        const Stand top = plan.makeStand({.x = 2, .y = 1, .width = 4, .height = 3});
        plan.insertStand(plan.size(), bottom);
        plan.insertStand(plan.size(), top);

        constexpr Vec2 kUnderBoth{.x = 3, .y = 2};
        constexpr Vec2 kUnderBottomOnly{.x = 0.5, .y = 0.5};
        constexpr Vec2 kUnderNone{.x = 30, .y = 30};
        QCOMPARE(plan.standAt(kUnderBoth), top.id);
        QCOMPARE(plan.standAt(kUnderBottomOnly), bottom.id);
        QVERIFY(!plan.standAt(kUnderNone));
    }
};

QTEST_APPLESS_MAIN(TestPlan)
#include "tst_plan.moc"
