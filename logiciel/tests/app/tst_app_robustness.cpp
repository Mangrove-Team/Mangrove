/*
// Mangrove Project
// tst_app_robustness.cpp
// Description: Everything QML can send to the app layer is untrusted: pointer
//              positions, property writes, call order. None of it may corrupt
//              the plan, the history or the view.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Editor.hpp"
#include "ViewTransform.hpp"
#include "constraints/Alert.h"
#include "models/alert/AlertModel.hpp"
#include "models/stand/StandModel.hpp"
#include "plan/Ids.h"
#include "plan/Plan.h"
#include "plan/Stand.h"

#include <QtTest>

#include <cmath>
#include <limits>

using namespace mangrove;
using namespace mangrove::app;

namespace {

constexpr double kNaN = std::numeric_limits<double>::quiet_NaN();
constexpr double kInf = std::numeric_limits<double>::infinity();

// Any finite coordinate, next to a non-finite one.
constexpr double kFinite = 100.0;
// Coordinates near the limits of double: adding a stand size or a span to them overflows.
constexpr double kHuge = 1e300;
constexpr double kNearMaxDouble = 1.7e308;
// Finite, but doubling it overflows.
constexpr double kOverflowsWhenDoubled = 1e308;
// A positive length that rounds to nothing at any zoom.
constexpr double kTiny = 1e-300;

// Positions in meters; px() turns them into screen pixels.
// The reference stand is placed by a click here.
constexpr QPointF kStandOrigin{10, 10};
// A point inside the reference stand, to grab it.
constexpr QPointF kInsideStand{11, 11};
// Points where no stand ever is, at increasing distance.
constexpr QPointF kNearOrigin{1, 1};
constexpr QPointF kEmptySpot{5, 5};
constexpr QPointF kDragEnd{6, 6};
constexpr QPointF kElsewhere{20, 20};
constexpr QPointF kFurther{25, 25};
constexpr QPointF kFarAway{30, 30};

// View inputs that are valid on their own.
constexpr QPointF kCursor{100, 100};
constexpr QRectF kHall{0, 0, 60, 40};
constexpr QSizeF kViewport{800, 600};
constexpr double kMargin = 24;
constexpr double kZoomIn = 2.0;
constexpr double kHugeZoom = 20.0;

// Default view: offset (0, 0), 16 px per meter, zoom 1.
QPointF px(QPointF meters)
{
    return meters * ViewTransform::kDefaultPixelsPerMeter;
}

void click(Editor &editor, QPointF position)
{
    editor.press(position);
    editor.release(position);
}

// Every stand exposed to QML has finite, non-negative geometry.
void verifyStandsAreSane(Editor &editor)
{
    const StandModel *model = editor.stands();
    for (int row = 0; row < model->rowCount(); ++row) {
        const QModelIndex index = model->index(row);
        const double left = model->data(index, StandModel::XmRole).toDouble();
        const double top = model->data(index, StandModel::YmRole).toDouble();
        const double width = model->data(index, StandModel::WidthMRole).toDouble();
        const double height = model->data(index, StandModel::HeightMRole).toDouble();
        QVERIFY2(std::isfinite(left) && std::isfinite(top) && std::isfinite(width)
                && std::isfinite(height),
            qPrintable(QStringLiteral("row %1 is not finite").arg(row)));
        QVERIFY(width >= 0 && height >= 0);
        QVERIFY(std::isfinite(left + width) && std::isfinite(top + height));
    }
}

void verifyViewIsSane(const ViewTransform &view)
{
    QVERIFY(std::isfinite(view.scale()) && view.scale() > 0);
    QVERIFY(view.zoom() >= ViewTransform::kMinZoom && view.zoom() <= ViewTransform::kMaxZoom);
    QVERIFY(std::isfinite(view.offset().x()) && std::isfinite(view.offset().y()));
}

} // namespace

class TestAppRobustness : public QObject {
    Q_OBJECT

private slots:
    void nonFinitePointerIsIgnored_data()
    {
        QTest::addColumn<QPointF>("bad");
        QTest::newRow("NaN") << QPointF(kNaN, kNaN);
        QTest::newRow("NaN x") << QPointF(kNaN, kFinite);
        QTest::newRow("+inf") << QPointF(kInf, kInf);
        QTest::newRow("-inf y") << QPointF(kFinite, -kInf);
    }

    // With each tool, around one valid stand: a non-finite click, then a valid press
    // followed by non-finite moves and release. The stand is untouched, no preview remains.
    void nonFinitePointerIsIgnored()
    {
        QFETCH(const QPointF, bad);

        for (const auto tool : {Editor::ToolKind::Stand, Editor::ToolKind::Select}) {
            Editor editor;
            editor.setActiveTool(Editor::ToolKind::Stand);
            click(editor, px(kStandOrigin));
            editor.setActiveTool(tool);

            click(editor, bad);
            editor.press(px(kInsideStand));
            editor.move(bad);
            editor.release(bad);
            editor.cancel();

            verifyStandsAreSane(editor);
            QVERIFY(!editor.hasPreview());
            QCOMPARE(editor.stands()->rowCount(), 1);
        }
    }

    void hugeCoordinatesNeverCreateInvalidStands_data()
    {
        QTest::addColumn<QPointF>("from");
        QTest::addColumn<QPointF>("target");
        QTest::newRow("far away click") << QPointF(kHuge, kHuge) << QPointF(kHuge, kHuge);
        QTest::newRow("edge of double")
            << QPointF(kNearMaxDouble, kNearMaxDouble) << QPointF(kNearMaxDouble, kNearMaxDouble);
        QTest::newRow("span overflows")
            << QPointF(-kNearMaxDouble, 0) << QPointF(kNearMaxDouble, kFinite);
    }

    // Drags near the limits of double never store a non-finite stand, and a rejected
    // stand must not leave an empty entry in the history.
    void hugeCoordinatesNeverCreateInvalidStands()
    {
        QFETCH(const QPointF, from);
        QFETCH(const QPointF, target);

        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        editor.press(from);
        editor.move(target);
        editor.release(target);

        verifyStandsAreSane(editor);
        QCOMPARE(editor.canUndo(), editor.stands()->rowCount() > 0);
    }

    // QML can write any int to activeTool: an unknown value is ignored and the editor
    // stays usable.
    void unknownToolIsIgnored()
    {
        Editor editor;
        QSignalSpy const changed(&editor, &Editor::activeToolChanged);
        constexpr int kUnknownTool = 42;
        QVERIFY(editor.setProperty("activeTool", kUnknownTool));
        QCOMPARE(editor.activeTool(), Editor::ToolKind::Select);
        QCOMPARE(changed.count(), 0);
        click(editor, px(kNearOrigin));
    }

    // Move or release without press and repeated cancels create nothing; a second
    // press without release restarts the gesture.
    void eventsOutOfOrderAreHarmless()
    {
        Editor editor;
        for (const auto tool : {Editor::ToolKind::Stand, Editor::ToolKind::Select}) {
            editor.setActiveTool(tool);
            editor.move(px(kEmptySpot));
            editor.release(px(kEmptySpot));
            editor.cancel();
            editor.cancel();
        }
        QCOMPARE(editor.stands()->rowCount(), 0);
        QVERIFY(!editor.canUndo());

        editor.setActiveTool(Editor::ToolKind::Stand);
        editor.press(px(kNearOrigin));
        editor.press(px(kElsewhere));
        editor.release(px(kElsewhere));
        QCOMPARE(editor.stands()->rowCount(), 1);
    }

    // Switching tool mid-drag cancels the gesture: no stand is created and a dragged
    // stand stays where it was.
    void switchingToolCancelsGesture()
    {
        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        editor.press(px(kNearOrigin));
        editor.move(px(kDragEnd));
        QVERIFY(editor.hasPreview());
        editor.setActiveTool(Editor::ToolKind::Select);
        QVERIFY(!editor.hasPreview());
        editor.release(px(kDragEnd));
        QCOMPARE(editor.stands()->rowCount(), 0);

        editor.setActiveTool(Editor::ToolKind::Stand);
        click(editor, px(kStandOrigin));
        editor.setActiveTool(Editor::ToolKind::Select);
        editor.press(px(kInsideStand));
        editor.move(px(kFarAway));
        editor.setActiveTool(Editor::ToolKind::Stand);
        const QModelIndex first = editor.stands()->index(0);
        QCOMPARE(editor.stands()->data(first, StandModel::XmRole).toDouble(), kStandOrigin.x());
    }

    // Undo during a drag cancels the gesture, then undoes the last step; the release
    // that follows the cancelled gesture does nothing.
    void undoMidGestureCancelsFirst()
    {
        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        click(editor, px(kStandOrigin));
        editor.press(px(kElsewhere));
        editor.move(px(kFurther));
        editor.undo();
        QVERIFY(!editor.hasPreview());
        QCOMPARE(editor.stands()->rowCount(), 0);
        editor.release(px(kFurther));
        QCOMPARE(editor.stands()->rowCount(), 0);
        QVERIFY(editor.canRedo());
    }

    // A new stand is selected; undone then redone, it comes back unselected.
    void undoClearsSelectionOfRemovedStand()
    {
        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        click(editor, px(kStandOrigin));
        editor.undo();
        editor.redo();
        const QModelIndex first = editor.stands()->index(0);
        QCOMPARE(editor.stands()->data(first, StandModel::SelectedRole).toBool(), false);
    }

    void undoRedoOnEmptyHistoryAreNoOps()
    {
        Editor editor;
        for (int i = 0; i < 3; ++i) {
            editor.undo();
            editor.redo();
        }
        QVERIFY(!editor.canUndo() && !editor.canRedo());
    }

    void viewTransformRejectsNonFiniteInput()
    {
        Editor editor;
        ViewTransform &view = *editor.view();
        QSignalSpy const changed(&view, &ViewTransform::transformChanged);

        for (const double bad : {kNaN, kInf, -kInf}) {
            view.setZoom(bad);
            view.setPixelsPerMeter(bad);
            view.setOffset({bad, 0});
            view.pan({0, bad});
            view.zoomAt(kCursor, bad);
            view.zoomAt({bad, kCursor.y()}, kZoomIn);
            view.fit({0, 0, bad, kHall.height()}, kViewport, kMargin);
            view.fit(kHall, {bad, kViewport.height()}, kMargin);
            view.fit(kHall, kViewport, bad);
            verifyViewIsSane(view);
        }
        for (const double bad : {0.0, -1.0}) {
            view.setPixelsPerMeter(bad);
            view.zoomAt(kCursor, bad);
            verifyViewIsSane(view);
        }
        QCOMPARE(changed.count(), 0);
    }

    // Extreme wheel deltas, a pan that would overflow to +inf and a fit on a tiny rect
    // (clamped to kMaxZoom) all keep the view finite.
    void viewTransformSurvivesExtremeInput()
    {
        Editor editor;
        ViewTransform &view = *editor.view();
        view.wheelZoomAt(kCursor, std::numeric_limits<int>::max());
        verifyViewIsSane(view);
        view.wheelZoomAt(kCursor, std::numeric_limits<int>::min());
        verifyViewIsSane(view);
        view.pan({kOverflowsWhenDoubled, kOverflowsWhenDoubled});
        view.pan({kOverflowsWhenDoubled, kOverflowsWhenDoubled});
        verifyViewIsSane(view);
        view.zoomAt({kOverflowsWhenDoubled, kOverflowsWhenDoubled}, kHugeZoom);
        verifyViewIsSane(view);
        view.fit({0, 0, kTiny, kTiny}, kViewport, kMargin);
        verifyViewIsSane(view);
    }

    // Labels will be user-editable: "%1"-style text must be printed verbatim.
    void alertMessageIsNotAFormatString()
    {
        core::Plan plan;
        plan.insertStand(0,
            core::Stand{.id = core::StandId{1},
                .label = "%2",
                .rect = {.x = 0, .y = 0, .width = 4, .height = 3}});
        plan.insertStand(1,
            core::Stand{.id = core::StandId{2},
                .label = "%1",
                .rect = {.x = 1, .y = 1, .width = 4, .height = 3}});
        const core::Alert overlap{
            .ruleId = "overlap", .stands = {core::StandId{1}, core::StandId{2}}};
        QCOMPARE(AlertModel::message(overlap, plan), QStringLiteral("Stands %2 and %1 overlap."));
    }

    void alertsNamingUnknownStandsAreHarmless()
    {
        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        click(editor, px(kStandOrigin));
        constexpr core::StandId kUnknownStand{999};
        editor.setAlerts({{"overlap", {core::StandId{}, kUnknownStand}}});

        QCOMPARE(editor.alerts()->count(), 1);
        QVERIFY(
            !editor.stands()->data(editor.stands()->index(0), StandModel::InConflictRole).toBool());
        QCOMPARE(
            editor.alerts()->data(editor.alerts()->index(0), AlertModel::MessageRole).toString(),
            QStringLiteral("Stands ? and ? overlap."));
    }

    void malformedAlertsAreDisplayable()
    {
        core::Plan const plan;
        QCOMPARE(
            AlertModel::message({"overlap", {}}, plan), QStringLiteral("Stands ? and ? overlap."));
        constexpr core::StandId kUnknownStand{99};
        QCOMPARE(AlertModel::message({"inside-hall", {kUnknownStand}}, plan),
            QStringLiteral("Stand ? is outside the hall."));
        QCOMPARE(AlertModel::message({"%1<b>", {}}, plan),
            QStringLiteral("Constraint \"%1<b>\" is not met."));
    }
};

QTEST_GUILESS_MAIN(TestAppRobustness)
#include "tst_app_robustness.moc"
