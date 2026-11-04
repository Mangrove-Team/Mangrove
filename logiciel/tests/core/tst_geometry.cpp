/*
// Mangrove Project
// tst_geometry.cpp
// Description: Tests of the core geometry primitives.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "geometry/Geometry.h"

#include <QtTest>

using namespace mangrove::core;

class TestGeometry : public QObject {
    Q_OBJECT

private slots:
    void containsPointIsInclusive()
    {
        const RectM rect{.x = 0, .y = 0, .width = 4, .height = 3};
        QVERIFY(rect.contains(Vec2{0, 0}));
        QVERIFY(rect.contains(Vec2{4, 3}));
        constexpr double kJustOutside = 0.01;
        QVERIFY(!rect.contains(Vec2{rect.right() + kJustOutside, 1}));
    }

    void containsRect()
    {
        const RectM hall{.x = 0, .y = 0, .width = 60, .height = 40};
        QVERIFY(hall.contains(RectM{56, 37, 4, 3}));
        QVERIFY(!hall.contains(RectM{57, 37, 4, 3}));
    }

    // Rects that only share an edge or a corner do not intersect; overlapping or
    // nested ones do.
    void intersectsIsStrict()
    {
        const RectM first{.x = 0, .y = 0, .width = 4, .height = 3};
        QVERIFY(!first.intersects(RectM{4, 0, 4, 3}));
        QVERIFY(!first.intersects(RectM{0, 3, 4, 3}));
        QVERIFY(!first.intersects(RectM{4, 3, 4, 3}));
        constexpr double kOverlap = 0.5;
        QVERIFY(first.intersects(RectM{first.right() - kOverlap, first.bottom() - kOverlap, 4, 3}));
        QVERIFY(first.intersects(RectM{1, 1, 1, 1}));
    }

    void translated()
    {
        constexpr Vec2 kDelta{.x = 1.5, .y = -2};
        QCOMPARE(RectM(1, 2, 4, 3).translated(kDelta), RectM(1 + kDelta.x, 2 + kDelta.y, 4, 3));
    }

    void fromCornersNormalizes()
    {
        QCOMPARE(RectM::fromCorners({5, 1}, {1, 4}), RectM(1, 1, 4, 3));
        QCOMPARE(RectM::fromCorners({1, 1}, {1, 1}), RectM(1, 1, 0, 0));
    }
};

QTEST_APPLESS_MAIN(TestGeometry)
#include "tst_geometry.moc"
