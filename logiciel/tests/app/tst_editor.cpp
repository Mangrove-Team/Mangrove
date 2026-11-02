/*
// Mangrove Project
// tst_editor.cpp
// Description: Drives Editor the way QML does (screen pixels in, models out).
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Editor.hpp"

#include <QSignalSpy>
#include <QtTest>

using namespace mangrove::app;

namespace {

// Default view: offset (0, 0), 16 px per meter, zoom 1.
QPointF px(double xm, double ym)
{
    return {xm * ViewTransform::kDefaultPixelsPerMeter, ym * ViewTransform::kDefaultPixelsPerMeter};
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

void drag(Editor &editor, QPointF from, QPointF to, int steps = 5)
{
    editor.press(from);
    for (int i = 1; i <= steps; ++i)
        editor.move(from + (to - from) * i / steps);
    editor.release(to);
}

} // namespace

class TestEditor : public QObject {
    Q_OBJECT

private slots:
    // A click with the Stand tool places a default 4 x 3 m stand, snapped to 0.5 m.
    void clickPlacesDefaultStand()
    {
        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        drag(editor, px(10.2, 10.1), px(10.2, 10.1), 0);

        QCOMPARE(editor.stands()->rowCount(), 1);
        QCOMPARE(standRect(editor, 0), QRectF(10, 10, 4, 3));
        QVERIFY(editor.canUndo());
        QVERIFY(!editor.hasPreview());
    }

    void dragDrawsStandWithPreview()
    {
        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        editor.press(px(2, 2));
        editor.move(px(8, 6.1));
        QVERIFY(editor.hasPreview());
        QCOMPARE(editor.preview(), QRectF(2, 2, 6, 4));
        editor.release(px(8, 6.1));

        QVERIFY(!editor.hasPreview());
        QCOMPARE(standRect(editor, 0), QRectF(2, 2, 6, 4));
    }

    // Dragging a stand with the Select tool moves and selects it. Undo reverts the
    // whole drag at once; a second undo removes the stand creation.
    void dragMovesStandAsOneUndoStep()
    {
        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        drag(editor, px(10, 10), px(10, 10), 0);
        editor.setActiveTool(Editor::ToolKind::Select);

        drag(editor, px(11, 11), px(14, 13), 10);
        QCOMPARE(standRect(editor, 0), QRectF(13, 12, 4, 3));
        QCOMPARE(editor.stands()->data(editor.stands()->index(0), StandModel::SelectedRole), true);

        editor.undo();
        QCOMPARE(standRect(editor, 0), QRectF(10, 10, 4, 3));
        editor.redo();
        QCOMPARE(standRect(editor, 0), QRectF(13, 12, 4, 3));

        editor.undo();
        editor.undo();
        QCOMPARE(editor.stands()->rowCount(), 0);
        QVERIFY(!editor.canUndo());
        editor.redo();
        QCOMPARE(editor.stands()->rowCount(), 1);
    }

    void twoDragsAreTwoUndoSteps()
    {
        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        drag(editor, px(10, 10), px(10, 10), 0);
        editor.setActiveTool(Editor::ToolKind::Select);
        drag(editor, px(11, 11), px(12, 11));
        drag(editor, px(12, 11), px(13, 11));

        editor.undo();
        QCOMPARE(standRect(editor, 0), QRectF(11, 10, 4, 3));
    }

    // Cancel puts a dragged stand back and leaves nothing in the history: the next
    // undo removes the stand creation.
    void cancelRestoresDraggedStand()
    {
        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        drag(editor, px(10, 10), px(10, 10), 0);
        editor.setActiveTool(Editor::ToolKind::Select);

        editor.press(px(11, 11));
        editor.move(px(20, 20));
        editor.cancel();
        QCOMPARE(standRect(editor, 0), QRectF(10, 10, 4, 3));

        editor.undo();
        QCOMPARE(editor.stands()->rowCount(), 0);
    }

    // An alert marks the stands it names as in conflict and notifies only the stands
    // whose state changed; clearing the alerts unmarks them.
    void alertsMarkTheStandsTheyName()
    {
        Editor editor;
        editor.setActiveTool(Editor::ToolKind::Stand);
        drag(editor, px(10, 10), px(10, 10), 0);
        drag(editor, px(30, 30), px(30, 30), 0);
        StandModel *stands = editor.stands();
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
        Editor editor;
        ViewTransform *view = editor.view();
        view->pan({37, -12});
        const QPointF cursor(300, 200);
        const QPointF before = view->toMeters(cursor);

        view->zoomAt(cursor, 2.5);
        QCOMPARE(view->scale(), 40.0);
        QCOMPARE(view->toMeters(cursor), before);

        view->wheelZoomAt(cursor, -120);
        QCOMPARE(view->toMeters(cursor), before);
        QCOMPARE(view->toScreen(view->toMeters(cursor)), cursor);
    }
};

QTEST_GUILESS_MAIN(TestEditor)
#include "tst_editor.moc"
