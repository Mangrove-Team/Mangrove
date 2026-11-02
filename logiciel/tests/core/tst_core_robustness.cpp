/*
// Mangrove Project
// tst_core_robustness.cpp
// Description: The core must stay consistent whatever it is fed: non-finite
//              or inverted rectangles, unknown or exhausted ids. Future file
//              loading will hand it untrusted data.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "plan/Plan.h"

#include <QtTest>

#include <limits>

using namespace mangrove::core;

namespace {

constexpr double kNaN = std::numeric_limits<double>::quiet_NaN();
constexpr double kInf = std::numeric_limits<double>::infinity();
constexpr std::uint64_t kMaxId = std::numeric_limits<std::uint64_t>::max();

} // namespace

class TestCoreRobustness : public QObject {
    Q_OBJECT

private slots:
    void rectValidity_data()
    {
        QTest::addColumn<RectM>("rect");
        QTest::addColumn<bool>("valid");

        QTest::newRow("regular") << RectM{1, 2, 4, 3} << true;
        QTest::newRow("empty") << RectM{1, 2, 0, 0} << true;
        QTest::newRow("negative width") << RectM{0, 0, -4, 3} << false;
        QTest::newRow("negative height") << RectM{0, 0, 4, -3} << false;
        QTest::newRow("NaN x") << RectM{kNaN, 0, 4, 3} << false;
        QTest::newRow("NaN width") << RectM{0, 0, kNaN, 3} << false;
        QTest::newRow("infinite y") << RectM{0, kInf, 4, 3} << false;
        QTest::newRow("infinite height") << RectM{0, 0, 4, kInf} << false;
        QTest::newRow("right edge overflows") << RectM{1e308, 0, 1e308, 3} << false;
    }

    void rectValidity()
    {
        QFETCH(RectM, rect);
        QFETCH(bool, valid);
        QCOMPARE(rect.isValid(), valid);
    }

    void planRejectsInvalidRects_data() { rectValidity_data(); }

    // An invalid rect is refused on insertion and on update; a refused update leaves
    // the stand untouched.
    void planRejectsInvalidRects()
    {
        QFETCH(RectM, rect);
        QFETCH(bool, valid);

        Plan plan;
        QCOMPARE(plan.insertStand(0, plan.makeStand(rect)), valid);

        const Stand reference = plan.makeStand({0, 0, 4, 3});
        QVERIFY(plan.insertStand(plan.size(), reference));
        QCOMPARE(plan.setStandRect(reference.id, rect), valid);
        if (!valid)
            QCOMPARE(plan.find(reference.id)->rect, RectM(0, 0, 4, 3));
    }

    void unknownIdsAreHarmless()
    {
        Plan plan;
        plan.insertStand(0, plan.makeStand({0, 0, 4, 3}));

        for (const StandId id : {StandId{}, StandId{999}, StandId{kMaxId}}) {
            QVERIFY(plan.find(id) == nullptr);
            QVERIFY(!plan.indexOf(id).has_value());
            QVERIFY(!plan.removeStand(id).has_value());
            QVERIFY(!plan.setStandRect(id, {1, 1, 1, 1}));
        }
        QCOMPARE(plan.size(), std::size_t{1});
    }

    void insertIndexIsClamped()
    {
        Plan plan;
        const Stand a = plan.makeStand({0, 0, 4, 3});
        QVERIFY(plan.insertStand(std::numeric_limits<std::size_t>::max(), a));
        QCOMPARE(plan.indexOf(a.id), std::optional<std::size_t>(0));
    }

    void standAtNonFinitePointFindsNothing()
    {
        Plan plan;
        plan.insertStand(0, plan.makeStand({0, 0, 4, 3}));
        QVERIFY(!plan.standAt({kNaN, 1}));
        QVERIFY(!plan.standAt({kInf, kInf}));
    }

    void foreignIdNeverCollidesWithNewOnes()
    {
        Plan plan;
        QVERIFY(plan.insertStand(0, Stand{StandId{7}, "S07", {0, 0, 4, 3}}));
        for (int i = 0; i < 10; ++i)
            QVERIFY(plan.makeStand({0, 0, 1, 1}).id.value > 7);
    }

    // Found by fuzz_plan: a stand from outside (file, paste...) already named
    // like the next generated label. Id 50 makes the next generated id 51.
    void generatedLabelSkipsLabelsInUse()
    {
        Plan plan;
        QVERIFY(plan.insertStand(0, Stand{StandId{50}, "S51", {0, 0, 4, 3}}));
        const Stand a = plan.makeStand({0, 0, 1, 1});
        QCOMPARE(a.id.value, std::uint64_t{51});
        QVERIFY(a.label != "S51");
        QVERIFY(plan.insertStand(plan.size(), a));
        const Stand b = plan.makeStand({0, 0, 1, 1});
        QVERIFY(b.label != "S51" && b.label != a.label);
    }

    // Once the last id is used, makeStand returns an invalid id instead of wrapping
    // around to 0, 1, ...
    void exhaustedIdsAreNeverReusedNorZero()
    {
        Plan plan;
        QVERIFY(plan.insertStand(0, Stand{StandId{kMaxId - 1}, "far", {0, 0, 1, 1}}));

        const Stand last = plan.makeStand({0, 0, 1, 1});
        QCOMPARE(last.id.value, kMaxId);
        QVERIFY(plan.insertStand(plan.size(), last));

        const Stand none = plan.makeStand({0, 0, 1, 1});
        QVERIFY(!none.id);
        QVERIFY(!plan.insertStand(plan.size(), none));
        QCOMPARE(plan.size(), std::size_t{2});
    }

    // Inserting kMaxId first leaves no id above it: the next generated id is invalid.
    void maxIdInsertedFirstExhaustsAllocation()
    {
        Plan plan;
        QVERIFY(plan.insertStand(0, Stand{StandId{kMaxId}, "max", {0, 0, 1, 1}}));
        const Stand next = plan.makeStand({0, 0, 1, 1});
        QVERIFY(!next.id || next.id.value > kMaxId);
    }
};

QTEST_APPLESS_MAIN(TestCoreRobustness)
#include "tst_core_robustness.moc"
