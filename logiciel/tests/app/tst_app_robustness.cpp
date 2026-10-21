/*
// Mangrove Project
// tst_app_robustness.cpp
// Description: Everything QML can send to the app layer is untrusted: pointer
//              positions, property writes, call order. None of it may corrupt
//              the plan, the history or the view.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Editor.hpp"
#include "models/alert/AlertModel.hpp"
#include "plan/Plan.h"

#include <QtTest>

#include <cmath>
#include <limits>

using namespace mangrove;
using namespace mangrove::app;

namespace {

constexpr double kNaN = std::numeric_limits<double>::quiet_NaN();
constexpr double kInf = std::numeric_limits<double>::infinity();

QPointF px(double xm, double ym)
{
    return {xm * ViewTransform::kDefaultPixelsPerMeter, ym * ViewTransform::kDefaultPixelsPerMeter};
}

void click(Editor &editor, QPointF at)
{
    editor.press(at);
    editor.release(at);
}

// Every stand exposed to QML has finite, non-negative geometry.
void verifyStandsAreSane(Editor &editor)
{
    const StandModel *model = editor.stands();
    for (int row = 0; row < model->rowCount(); ++row) {
        const QModelIndex index = model->index(row);
        const double x = model->data(index, StandModel::XmRole).toDouble();
        const double y = model->data(index, StandModel::YmRole).toDouble();
        const double w = model->data(index, StandModel::WidthMRole).toDouble();
        const double h = model->data(index, StandModel::HeightMRole).toDouble();
        QVERIFY2(std::isfinite(x) && std::isfinite(y) && std::isfinite(w) && std::isfinite(h),
            qPrintable(QStringLiteral("row %1 is not finite").arg(row)));
        QVERIFY(w >= 0 && h >= 0);
        QVERIFY(std::isfinite(x + w) && std::isfinite(y + h));
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
        QTest::newRow("NaN x") << QPointF(kNaN, 100);
        QTest::newRow("+inf") << QPointF(kInf, kInf);
        QTest::newRow("-inf y") << QPointF(100, -kInf);
    }

    // With each tool, around one valid stand: a non-finite click, then a valid press
    // followed by non-finite moves and release. The stand is untouched, no preview remains.
    void nonFinitePointerIsIgnored()
    {
        QFETCH(QPointF, bad);

        for (const auto tool : {Editor::ToolKind::Stand, Editor::ToolKind::Select}) {
            Editor editor;
            editor.setActiveTool(Editor::ToolKind::Stand);
            click(editor, px(10, 10));
            editor.setActiveTool(tool);

            click(editor, bad);
            editor.press(px(11, 11));
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
        QTest::addColumn<QPointF>("to");
        QTest::newRow("far away click") << QPointF(1e300, 1e300) << QPointF(1e300, 1e300);
        QTest::newRow("edge of double") << QPointF(1.7e308, 1.7e308) << QPointF(1.7e308, 1.7e308);
        QTest::newRow("span overflows") << QPointF(-1.7e308, 0) << QPointF(1.7e308, 100);
    }

    // Drags near the limits of double never store a non-finite stand, and a rejected
    // stand must not leave an empty entry in the history.
    void hugeCoordinatesNeverCreateInvalidStands()
    {
        QFETCH(QPointF, from);
        QFETCH(QPointF, to);

        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        editor.press(from);
        editor.move(to);
        editor.release(to);

        verifyStandsAreSane(editor);
        QCOMPARE(editor.canUndo(), editor.stands()->rowCount() > 0);
    }

    // QML can write any int to activeTool: an unknown value is ignored and the editor
    // stays usable.
    void unknownToolIsIgnored()
    {
        Editor editor;
        QSignalSpy changed(&editor, &Editor::activeToolChanged);
        editor.setActiveTool(static_cast<Editor::ToolKind>(42));
        QCOMPARE(editor.activeTool(), Editor::ToolKind::Select);
        QCOMPARE(changed.count(), 0);
        click(editor, px(1, 1));
    }

    // Move or release without press and repeated cancels create nothing; a second
    // press without release restarts the gesture.
    void eventsOutOfOrderAreHarmless()
    {
        Editor editor;
        for (const auto tool : {Editor::ToolKind::Stand, Editor::ToolKind::Select}) {
            editor.setActiveTool(tool);
            editor.move(px(5, 5));
            editor.release(px(5, 5));
            editor.cancel();
            editor.cancel();
        }
        QCOMPARE(editor.stands()->rowCount(), 0);
        QVERIFY(!editor.canUndo());

        editor.setActiveTool(Editor::ToolKind::Stand);
        editor.press(px(1, 1));
        editor.press(px(20, 20));
        editor.release(px(20, 20));
        QCOMPARE(editor.stands()->rowCount(), 1);
    }

    // Switching tool mid-drag cancels the gesture: no stand is created and a dragged
    // stand stays where it was.
    void switchingToolCancelsGesture()
    {
        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        editor.press(px(1, 1));
        editor.move(px(6, 6));
        QVERIFY(editor.hasPreview());
        editor.setActiveTool(Editor::ToolKind::Select);
        QVERIFY(!editor.hasPreview());
        editor.release(px(6, 6));
        QCOMPARE(editor.stands()->rowCount(), 0);

        editor.setActiveTool(Editor::ToolKind::Stand);
        click(editor, px(10, 10));
        editor.setActiveTool(Editor::ToolKind::Select);
        editor.press(px(11, 11));
        editor.move(px(30, 30));
        editor.setActiveTool(Editor::ToolKind::Stand);
        const QModelIndex first = editor.stands()->index(0);
        QCOMPARE(editor.stands()->data(first, StandModel::XmRole).toDouble(), 10.0);
    }

    // Undo during a drag cancels the gesture, then undoes the last step; the release
    // that follows the cancelled gesture does nothing.
    void undoMidGestureCancelsFirst()
    {
        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        click(editor, px(10, 10));
        editor.press(px(20, 20));
        editor.move(px(25, 25));
        editor.undo();
        QVERIFY(!editor.hasPreview());
        QCOMPARE(editor.stands()->rowCount(), 0);
        editor.release(px(25, 25));
        QCOMPARE(editor.stands()->rowCount(), 0);
        QVERIFY(editor.canRedo());
    }

    // A new stand is selected; undone then redone, it comes back unselected.
    void undoClearsSelectionOfRemovedStand()
    {
        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        click(editor, px(10, 10));
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
        QSignalSpy changed(&view, &ViewTransform::transformChanged);

        for (const double bad : {kNaN, kInf, -kInf}) {
            view.setZoom(bad);
            view.setPixelsPerMeter(bad);
            view.setOffset({bad, 0});
            view.pan({0, bad});
            view.zoomAt({100, 100}, bad);
            view.zoomAt({bad, 100}, 2.0);
            view.fit({0, 0, bad, 40}, {800, 600}, 24);
            view.fit({0, 0, 60, 40}, {bad, 600}, 24);
            view.fit({0, 0, 60, 40}, {800, 600}, bad);
            verifyViewIsSane(view);
        }
        for (const double bad : {0.0, -1.0}) {
            view.setPixelsPerMeter(bad);
            view.zoomAt({100, 100}, bad);
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
        view.wheelZoomAt({100, 100}, std::numeric_limits<int>::max());
        verifyViewIsSane(view);
        view.wheelZoomAt({100, 100}, std::numeric_limits<int>::min());
        verifyViewIsSane(view);
        view.pan({1e308, 1e308});
        view.pan({1e308, 1e308});
        verifyViewIsSane(view);
        view.zoomAt({1e308, 1e308}, 20.0);
        verifyViewIsSane(view);
        view.fit({0, 0, 1e-300, 1e-300}, {800, 600}, 24);
        verifyViewIsSane(view);
    }

    // Labels will be user-editable: "%1"-style text must be printed verbatim.
    void alertMessageIsNotAFormatString()
    {
        core::Plan plan;
        plan.insertStand(0, core::Stand{core::StandId{1}, "%2", {0, 0, 4, 3}});
        plan.insertStand(1, core::Stand{core::StandId{2}, "%1", {1, 1, 4, 3}});
        const core::Alert overlap{"overlap", {core::StandId{1}, core::StandId{2}}};
        QCOMPARE(AlertModel::message(overlap, plan), QStringLiteral("Stands %2 and %1 overlap."));
    }

    void alertsNamingUnknownStandsAreHarmless()
    {
        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        click(editor, px(10, 10));
        editor.setAlerts({{"overlap", {core::StandId{}, core::StandId{999}}}});

        QCOMPARE(editor.alerts()->count(), 1);
        QVERIFY(
            !editor.stands()->data(editor.stands()->index(0), StandModel::InConflictRole).toBool());
        QCOMPARE(
            editor.alerts()->data(editor.alerts()->index(0), AlertModel::MessageRole).toString(),
            QStringLiteral("Stands ? and ? overlap."));
    }

    void malformedAlertsAreDisplayable()
    {
        core::Plan plan;
        QCOMPARE(
            AlertModel::message({"overlap", {}}, plan), QStringLiteral("Stands ? and ? overlap."));
        QCOMPARE(AlertModel::message({"inside-hall", {core::StandId{99}}}, plan),
            QStringLiteral("Stand ? is outside the hall."));
        QCOMPARE(AlertModel::message({"%1<b>", {}}, plan),
            QStringLiteral("Constraint \"%1<b>\" is not met."));
    }
};

QTEST_GUILESS_MAIN(TestAppRobustness)
#include "tst_app_robustness.moc"
