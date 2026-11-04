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
        const RectM r{0, 0, 4, 3};
        QVERIFY(r.contains(Vec2{0, 0}));
        QVERIFY(r.contains(Vec2{4, 3}));
        QVERIFY(!r.contains(Vec2{4.01, 1}));
    }

    void containsRect()
    {
        const RectM hall{0, 0, 60, 40};
        QVERIFY(hall.contains(RectM{56, 37, 4, 3}));
        QVERIFY(!hall.contains(RectM{57, 37, 4, 3}));
    }

    // Rects that only share an edge or a corner do not intersect; overlapping or
    // nested ones do.
    void intersectsIsStrict()
    {
        const RectM a{0, 0, 4, 3};
        QVERIFY(!a.intersects(RectM{4, 0, 4, 3}));
        QVERIFY(!a.intersects(RectM{0, 3, 4, 3}));
        QVERIFY(!a.intersects(RectM{4, 3, 4, 3}));
        QVERIFY(a.intersects(RectM{3.5, 2.5, 4, 3}));
        QVERIFY(a.intersects(RectM{1, 1, 1, 1}));
    }

    void translated() { QCOMPARE(RectM(1, 2, 4, 3).translated({1.5, -2}), RectM(2.5, 0, 4, 3)); }

    void fromCornersNormalizes()
    {
        QCOMPARE(RectM::fromCorners({5, 1}, {1, 4}), RectM(1, 1, 4, 3));
        QCOMPARE(RectM::fromCorners({1, 1}, {1, 1}), RectM(1, 1, 0, 0));
    }
};

QTEST_APPLESS_MAIN(TestGeometry)
#include "tst_geometry.moc"
