/*
// Mangrove Project
// tst_editor.cpp
// Description: Drives Editor the way QML does (screen pixels in, models out).
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Editor.hpp"
#include "ViewTransform.hpp"
#include "models/stand/StandModel.hpp"
#include "plan/Ids.h"

#include <QSignalSpy>
#include <QtTest>

using namespace mangrove::app;

namespace {

// Positions are in meters; px() turns them into screen pixels.
// A click with the Stand tool here places the reference stand.
constexpr QPointF kStandOrigin{10, 10};
// A point inside the reference stand, to grab it.
constexpr QPointF kInsideStand{11, 11};
constexpr QPointF kOtherStandOrigin{30, 30};
// What the Stand tool places on a simple click.
constexpr QSizeF kDefaultStandSize{4, 3};
constexpr int kDefaultDragMoves = 5;
// One wheel notch, in eighths of a degree (QWheelEvent::angleDelta).
constexpr int kWheelNotch = 120;

// Default view: offset (0, 0), 16 px per meter, zoom 1.
QPointF px(QPointF meters)
{
    return meters * ViewTransform::kDefaultPixelsPerMeter;
}

QRectF defaultStandAt(QPointF topLeft)
{
    return {topLeft, kDefaultStandSize};
}

QRectF standRect(Editor &editor, int row)
{
    const StandModel *model = editor.stands();
    const QModelIndex index = model->index(row);
    return {model->data(index, StandModel::XmRole).toDouble(),
        model->data(index, StandModel::YmRole).toDouble(),
        model->data(index, StandModel::WidthMRole).toDouble(),
        model->data(index, StandModel::HeightMRole).toDouble()};
}

// Press at `from`, `moves` intermediate moves, release at `target` (meters).
void drag(Editor &editor, QPointF from, QPointF target, int moves = kDefaultDragMoves)
{
    editor.press(px(from));
    for (int move = 1; move <= moves; ++move) {
        editor.move(px(from + (target - from) * move / moves));
    }
    editor.release(px(target));
}

void click(Editor &editor, QPointF position)
{
    drag(editor, position, position, 0);
}

} // namespace

class TestEditor : public QObject {
    Q_OBJECT

private slots:
    // A click with the Stand tool places a default 4 x 3 m stand, snapped to 0.5 m.
    void clickPlacesDefaultStand()
    {
        constexpr QPointF kOffGrid{10.2, 10.1};
        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        click(editor, kOffGrid);

        QCOMPARE(editor.stands()->rowCount(), 1);
        QCOMPARE(standRect(editor, 0), defaultStandAt(kStandOrigin));
        QVERIFY(editor.canUndo());
        QVERIFY(!editor.hasPreview());
    }

    // The far corner is off the 0.5 m grid: the stand snaps to 6 x 4 m.
    void dragDrawsStandWithPreview()
    {
        constexpr QPointF kCorner{2, 2};
        constexpr QPointF kFarCornerOffGrid{8, 6.1};
        const QRectF expected(kCorner, QPointF(8, 6));
        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        editor.press(px(kCorner));
        editor.move(px(kFarCornerOffGrid));
        QVERIFY(editor.hasPreview());
        QCOMPARE(editor.preview(), expected);
        editor.release(px(kFarCornerOffGrid));

        QVERIFY(!editor.hasPreview());
        QCOMPARE(standRect(editor, 0), expected);
    }

    // Dragging a stand with the Select tool moves and selects it. Undo reverts the
    // whole drag at once; a second undo removes the stand creation.
    void dragMovesStandAsOneUndoStep()
    {
        constexpr QPointF kOffset{3, 2};
        constexpr int kMoves = 10;
        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        click(editor, kStandOrigin);
        editor.setActiveTool(Editor::ToolKind::Select);

        drag(editor, kInsideStand, kInsideStand + kOffset, kMoves);
        QCOMPARE(standRect(editor, 0), defaultStandAt(kStandOrigin + kOffset));
        QCOMPARE(editor.stands()->data(editor.stands()->index(0), StandModel::SelectedRole), true);

        editor.undo();
        QCOMPARE(standRect(editor, 0), defaultStandAt(kStandOrigin));
        editor.redo();
        QCOMPARE(standRect(editor, 0), defaultStandAt(kStandOrigin + kOffset));

        editor.undo();
        editor.undo();
        QCOMPARE(editor.stands()->rowCount(), 0);
        QVERIFY(!editor.canUndo());
        editor.redo();
        QCOMPARE(editor.stands()->rowCount(), 1);
    }

    void twoDragsAreTwoUndoSteps()
    {
        constexpr QPointF kStep{1, 0};
        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        click(editor, kStandOrigin);
        editor.setActiveTool(Editor::ToolKind::Select);
        drag(editor, kInsideStand, kInsideStand + kStep);
        drag(editor, kInsideStand + kStep, kInsideStand + 2 * kStep);

        editor.undo();
        QCOMPARE(standRect(editor, 0), defaultStandAt(kStandOrigin + kStep));
    }

    // Cancel puts a dragged stand back and leaves nothing in the history: the next
    // undo removes the stand creation.
    void cancelRestoresDraggedStand()
    {
        constexpr QPointF kFarAway{20, 20};
        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        click(editor, kStandOrigin);
        editor.setActiveTool(Editor::ToolKind::Select);

        editor.press(px(kInsideStand));
        editor.move(px(kFarAway));
        editor.cancel();
        QCOMPARE(standRect(editor, 0), defaultStandAt(kStandOrigin));

        editor.undo();
        QCOMPARE(editor.stands()->rowCount(), 0);
    }

    // An alert marks the stands it names as in conflict and notifies only the stands
    // whose state changed; clearing the alerts unmarks them.
    void alertsMarkTheStandsTheyName()
    {
        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        click(editor, kStandOrigin);
        click(editor, kOtherStandOrigin);
        StandModel const *stands = editor.stands();
        const auto inConflict = [stands](int row) {
            return stands->data(stands->index(row), StandModel::InConflictRole).toBool();
        };
        const mangrove::core::StandId second{
            stands->data(stands->index(1), StandModel::StandIdRole).toULongLong()};
        QSignalSpy changed(stands, &StandModel::dataChanged);

        editor.setAlerts({{"inside-hall", {second}}});
        QCOMPARE(editor.alerts()->count(), 1);
        QVERIFY(!inConflict(0));
        QVERIFY(inConflict(1));
        QCOMPARE(changed.count(), 1);
        QCOMPARE(changed.first().first().toModelIndex().row(), 1);

        editor.setAlerts({});
        QCOMPARE(editor.alerts()->count(), 0);
        QVERIFY(!inConflict(1));
    }

    void zoomKeepsPointUnderCursor()
    {
        constexpr QPointF kPan{37, -12};
        constexpr QPointF kCursor{300, 200};
        constexpr double kZoom = 2.5;
        Editor editor;
        ViewTransform *view = editor.view();
        view->pan(kPan);
        const QPointF before = view->toMeters(kCursor);

        view->zoomAt(kCursor, kZoom);
        QCOMPARE(view->scale(), ViewTransform::kDefaultPixelsPerMeter * kZoom);
        QCOMPARE(view->toMeters(kCursor), before);

        view->wheelZoomAt(kCursor, -kWheelNotch);
        QCOMPARE(view->toMeters(kCursor), before);
        QCOMPARE(view->toScreen(view->toMeters(kCursor)), kCursor);
    }
};

QTEST_GUILESS_MAIN(TestEditor)
#include "tst_editor.moc"
