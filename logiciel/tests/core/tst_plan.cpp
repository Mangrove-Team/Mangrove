/*
// Mangrove Project
// tst_plan.cpp
// Description: Tests of mangrove::core::Plan.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "plan/Plan.h"

#include <QtTest>

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
        for (int i = 0; i < 12; ++i) {
            const Stand stand = plan.makeStand({0, 0, 4, 3});
            QVERIFY(static_cast<bool>(stand.id));
            ids.insert(stand.id.value);
            labels.insert(stand.label);
        }
        QCOMPARE(ids.size(), std::size_t{12});
        QCOMPARE(labels.size(), std::size_t{12});
        QVERIFY(labels.contains("S01"));
        QVERIFY(labels.contains("S12"));
    }

    void makeStandDoesNotInsert()
    {
        Plan plan;
        const Stand stand = plan.makeStand({0, 0, 4, 3});
        QCOMPARE(plan.size(), std::size_t{0});
        QVERIFY(plan.find(stand.id) == nullptr);
    }

    // Insertion at an index keeps the stack order; a duplicate or invalid id is refused.
    void insertAndRemove()
    {
        Plan plan;
        const Stand a = plan.makeStand({0, 0, 4, 3});
        const Stand b = plan.makeStand({10, 0, 4, 3});
        QVERIFY(plan.insertStand(plan.size(), a));
        QVERIFY(plan.insertStand(0, b));
        QVERIFY(!plan.insertStand(0, a));
        QVERIFY(!plan.insertStand(0, Stand{}));

        QCOMPARE(plan.indexOf(b.id), std::optional<std::size_t>(0));
        QCOMPARE(plan.indexOf(a.id), std::optional<std::size_t>(1));

        const auto removed = plan.removeStand(b.id);
        QVERIFY(removed.has_value());
        QCOMPARE(*removed, b);
        QCOMPARE(plan.size(), std::size_t{1});
        QVERIFY(plan.find(b.id) == nullptr);
        QVERIFY(!plan.removeStand(b.id).has_value());
    }

    void reinsertedIdIsNeverReallocated()
    {
        Plan source;
        const Stand far = Stand{StandId{42}, "S42", {0, 0, 4, 3}};
        QVERIFY(source.insertStand(0, far));
        QVERIFY(source.makeStand({0, 0, 1, 1}).id.value > 42);
    }

    void setStandRect()
    {
        Plan plan;
        const Stand a = plan.makeStand({0, 0, 4, 3});
        plan.insertStand(0, a);
        QVERIFY(plan.setStandRect(a.id, {5, 5, 4, 3}));
        QCOMPARE(plan.find(a.id)->rect, RectM(5, 5, 4, 3));
        QVERIFY(!plan.setStandRect(StandId{999}, {0, 0, 1, 1}));
    }

    // Where several stands cover a point, standAt returns the topmost one.
    void standAtReturnsTopmost()
    {
        Plan plan;
        const Stand bottom = plan.makeStand({0, 0, 4, 3});
        const Stand top = plan.makeStand({2, 1, 4, 3});
        plan.insertStand(plan.size(), bottom);
        plan.insertStand(plan.size(), top);

        QCOMPARE(plan.standAt({3, 2}), top.id);
        QCOMPARE(plan.standAt({0.5, 0.5}), bottom.id);
        QVERIFY(!plan.standAt({30, 30}));
    }
};

QTEST_APPLESS_MAIN(TestPlan)
#include "tst_plan.moc"
