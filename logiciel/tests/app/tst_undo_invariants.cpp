/*
// Mangrove Project
// tst_undo_invariants.cpp
// Description: Random editing sessions through Editor. Checks that the models
//              always respect the QAbstractItemModel contract and that undoing
//              everything then redoing everything restores the exact plan.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "TestSeed.h"

#include "Editor.hpp"

#include <QAbstractItemModelTester>
#include <QtTest>

#include <random>
#include <tuple>
#include <vector>

using namespace mangrove::app;

namespace {

using Row = std::tuple<quint64, QString, double, double, double, double>;

std::vector<Row> snapshot(Editor &editor)
{
    const StandModel *model = editor.stands();
    std::vector<Row> rows;
    for (int row = 0; row < model->rowCount(); ++row) {
        const QModelIndex i = model->index(row);
        rows.emplace_back(model->data(i, StandModel::StandIdRole).toULongLong(),
            model->data(i, StandModel::LabelRole).toString(),
            model->data(i, StandModel::XmRole).toDouble(),
            model->data(i, StandModel::YmRole).toDouble(),
            model->data(i, StandModel::WidthMRole).toDouble(),
            model->data(i, StandModel::HeightMRole).toDouble());
    }
    return rows;
}

} // namespace

class TestUndoInvariants : public QObject {
    Q_OBJECT

private slots:
    // Random session of tool switches, full and interrupted gestures, undo, redo, zoom
    // and alerts naming random stands. QAbstractItemModelTester fails the test as soon
    // as a model emits an inconsistent signal sequence. Undoing everything then empties
    // the plan, and redoing exactly what was undone (the session may have ended with
    // undos) restores it. Alerts are set from outside: undo and redo leave them alone.
    void randomSessionIsFullyUndoable()
    {
        std::mt19937 rng(mangrove::test::testSeed());
        std::uniform_int_distribution<int> action(0, 10);
        std::uniform_real_distribution<double> pixel(-50.0, 1100.0);
        const auto randomPoint = [&] { return QPointF(pixel(rng), pixel(rng)); };

        Editor editor;
        QAbstractItemModelTester standsTester(
            editor.stands(), QAbstractItemModelTester::FailureReportingMode::QtTest);
        QAbstractItemModelTester alertsTester(
            editor.alerts(), QAbstractItemModelTester::FailureReportingMode::QtTest);

        for (int step = 0; step < 1500; ++step) {
            switch (action(rng)) {
            case 0:
                editor.setActiveTool(Editor::ToolKind::Stand);
                break;
            case 1:
                editor.setActiveTool(Editor::ToolKind::Select);
                break;
            case 2:
            case 3:
            case 4: {
                const QPointF from = randomPoint();
                const QPointF to = randomPoint();
                editor.press(from);
                for (int i = 1; i <= 4; ++i)
                    editor.move(from + (to - from) * i / 4);
                editor.release(to);
                break;
            }
            case 5:
                editor.press(randomPoint());
                editor.move(randomPoint());
                editor.cancel();
                break;
            case 6:
            case 7:
                editor.undo();
                break;
            case 8:
                editor.redo();
                break;
            case 9:
                editor.view()->wheelZoomAt(randomPoint(), (action(rng) - 5) * 120);
                break;
            case 10: {
                std::vector<mangrove::core::Alert> alerts;
                const StandModel *stands = editor.stands();
                for (int row = 0; row < stands->rowCount(); ++row) {
                    if (action(rng) < 3)
                        alerts.push_back({"overlap",
                            {mangrove::core::StandId{
                                stands->data(stands->index(row), StandModel::StandIdRole)
                                    .toULongLong()}}});
                }
                editor.setAlerts(alerts);
                break;
            }
            }
            QCOMPARE(editor.alerts()->count(), editor.alerts()->rowCount());
            if (QTest::currentTestFailed())
                return;
        }

        const auto finalRows = snapshot(editor);
        const int finalAlerts = editor.alerts()->count();

        int undone = 0;
        while (editor.canUndo()) {
            editor.undo();
            ++undone;
        }
        QCOMPARE(editor.stands()->rowCount(), 0);
        QCOMPARE(editor.alerts()->count(), finalAlerts);

        for (int i = 0; i < undone; ++i)
            editor.redo();
        QVERIFY(snapshot(editor) == finalRows);
        QCOMPARE(editor.alerts()->count(), finalAlerts);
    }
};

QTEST_GUILESS_MAIN(TestUndoInvariants)
#include "tst_undo_invariants.moc"
