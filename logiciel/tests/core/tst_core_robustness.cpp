/*
// Mangrove Project
// tst_core_robustness.cpp
// Description: The core must stay consistent whatever it is fed: non-finite
//              or inverted rectangles, unknown or exhausted ids. Future file
//              loading will hand it untrusted data.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "geometry/Geometry.h"
#include "plan/Plan.h"
#include "plan/Stand.h"

#include <QtTest>

#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>

using namespace mangrove::core;

namespace {

constexpr double kNaN = std::numeric_limits<double>::quiet_NaN();
constexpr double kInf = std::numeric_limits<double>::infinity();
constexpr std::uint64_t kMaxId = std::numeric_limits<std::uint64_t>::max();
// Finite, but twice of it is not: x + width overflows to infinity.
constexpr double kHuge = 1e308;

} // namespace

class TestCoreRobustness : public QObject {
    Q_OBJECT

private slots:
    void rectValidity_data()
    {
        QTest::addColumn<RectM>("rect");
        QTest::addColumn<bool>("valid");

        QTest::newRow("regular") << RectM{.x = 1, .y = 2, .width = 4, .height = 3} << true;
        QTest::newRow("empty") << RectM{.x = 1, .y = 2, .width = 0, .height = 0} << true;
        QTest::newRow("negative width") << RectM{.x = 0, .y = 0, .width = -4, .height = 3} << false;
        QTest::newRow("negative height")
            << RectM{.x = 0, .y = 0, .width = 4, .height = -3} << false;
        QTest::newRow("NaN x") << RectM{.x = kNaN, .y = 0, .width = 4, .height = 3} << false;
        QTest::newRow("NaN width") << RectM{.x = 0, .y = 0, .width = kNaN, .height = 3} << false;
        QTest::newRow("infinite y") << RectM{.x = 0, .y = kInf, .width = 4, .height = 3} << false;
        QTest::newRow("infinite height")
            << RectM{.x = 0, .y = 0, .width = 4, .height = kInf} << false;
        QTest::newRow("right edge overflows")
            << RectM{.x = kHuge, .y = 0, .width = kHuge, .height = 3} << false;
    }

    void rectValidity()
    {
        QFETCH(const RectM, rect);
        QFETCH(const bool, valid);
        QCOMPARE(rect.isValid(), valid);
    }

    void planRejectsInvalidRects_data() { rectValidity_data(); }

    // An invalid rect is refused on insertion and on update; a refused update leaves
    // the stand untouched.
    void planRejectsInvalidRects()
    {
        QFETCH(const RectM, rect);
        QFETCH(const bool, valid);

        Plan plan;
        QCOMPARE(plan.insertStand(0, plan.makeStand(rect)), valid);

        const Stand reference = plan.makeStand({.x = 0, .y = 0, .width = 4, .height = 3});
        QVERIFY(plan.insertStand(plan.size(), reference));
        QCOMPARE(plan.setStandRect(reference.id, rect), valid);
        if (!valid) {
            QCOMPARE(plan.find(reference.id)->rect, RectM(0, 0, 4, 3));
        }
    }

    void unknownIdsAreHarmless()
    {
        Plan plan;
        plan.insertStand(0, plan.makeStand({.x = 0, .y = 0, .width = 4, .height = 3}));

        for (const StandId standId : {StandId{}, StandId{999}, StandId{kMaxId}}) {
            QVERIFY(plan.find(standId) == nullptr);
            QVERIFY(!plan.indexOf(standId).has_value());
            QVERIFY(!plan.removeStand(standId).has_value());
            QVERIFY(!plan.setStandRect(standId, {1, 1, 1, 1}));
        }
        QCOMPARE(plan.size(), std::size_t{1});
    }

    void insertIndexIsClamped()
    {
        Plan plan;
        const Stand first = plan.makeStand({.x = 0, .y = 0, .width = 4, .height = 3});
        QVERIFY(plan.insertStand(std::numeric_limits<std::size_t>::max(), first));
        QCOMPARE(plan.indexOf(first.id), std::optional<std::size_t>(0));
    }

    void standAtNonFinitePointFindsNothing()
    {
        Plan plan;
        plan.insertStand(0, plan.makeStand({.x = 0, .y = 0, .width = 4, .height = 3}));
        QVERIFY(!plan.standAt({kNaN, 1}));
        QVERIFY(!plan.standAt({kInf, kInf}));
    }

    void foreignIdNeverCollidesWithNewOnes()
    {
        Plan plan;
        QVERIFY(plan.insertStand(0, Stand{StandId{7}, "S07", {0, 0, 4, 3}}));
        constexpr int kNewStands = 10;
        for (int i = 0; i < kNewStands; ++i) {
            QVERIFY(plan.makeStand({0, 0, 1, 1}).id.value > 7);
        }
    }

    // Found by fuzz_plan: a stand from outside (file, paste...) already named
    // like the next generated label. Id 50 makes the next generated id 51.
    void generatedLabelSkipsLabelsInUse()
    {
        Plan plan;
        QVERIFY(plan.insertStand(0, Stand{StandId{50}, "S51", {0, 0, 4, 3}}));
        const Stand first = plan.makeStand({.x = 0, .y = 0, .width = 1, .height = 1});
        QCOMPARE(first.id.value, std::uint64_t{51});
        QVERIFY(first.label != "S51");
        QVERIFY(plan.insertStand(plan.size(), first));
        const Stand second = plan.makeStand({.x = 0, .y = 0, .width = 1, .height = 1});
        QVERIFY(second.label != "S51" && second.label != first.label);
    }

    // Once the last id is used, makeStand returns an invalid id instead of wrapping
    // around to 0, 1, ...
    void exhaustedIdsAreNeverReusedNorZero()
    {
        Plan plan;
        QVERIFY(plan.insertStand(0, Stand{StandId{kMaxId - 1}, "far", {0, 0, 1, 1}}));

        const Stand last = plan.makeStand({.x = 0, .y = 0, .width = 1, .height = 1});
        QCOMPARE(last.id.value, kMaxId);
        QVERIFY(plan.insertStand(plan.size(), last));

        const Stand none = plan.makeStand({.x = 0, .y = 0, .width = 1, .height = 1});
        QVERIFY(!none.id);
        QVERIFY(!plan.insertStand(plan.size(), none));
        QCOMPARE(plan.size(), std::size_t{2});
    }

    // Inserting kMaxId first leaves no id above it: the next generated id is invalid.
    void maxIdInsertedFirstExhaustsAllocation()
    {
        Plan plan;
        QVERIFY(plan.insertStand(0, Stand{StandId{kMaxId}, "max", {0, 0, 1, 1}}));
        const Stand next = plan.makeStand({.x = 0, .y = 0, .width = 1, .height = 1});
        QVERIFY(!next.id || next.id.value > kMaxId);
    }
};

QTEST_APPLESS_MAIN(TestCoreRobustness)
#include "tst_core_robustness.moc"
