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
#include "constraints/Alert.h"
#include "models/stand/StandModel.hpp"
#include "plan/Ids.h"

#include <QAbstractItemModelTester>
#include <QtTest>

#include <array>
#include <cstddef>
#include <cstdint>
#include <random>
#include <tuple>
#include <vector>

using namespace mangrove::app;

namespace {

constexpr int kSteps = 1500;
// Pointer positions reach slightly outside a default-sized view.
constexpr double kMinPixel = -50.0;
constexpr double kMaxPixel = 1100.0;
constexpr int kDragMoves = 4;
// One wheel notch, in eighths of a degree (QWheelEvent::angleDelta).
constexpr int kWheelNotch = 120;
constexpr int kMaxWheelNotches = 5;
constexpr double kAlertChance = 3.0 / 11.0;

enum class Action : std::uint8_t {
    UseStandTool,
    UseSelectTool,
    Drag,
    CancelledDrag,
    Undo,
    Redo,
    Zoom,
    SetAlerts,
};

// Drawn uniformly: an action listed three times is three times as likely.
constexpr std::array kActions{Action::UseStandTool, Action::UseSelectTool, Action::Drag,
    Action::Drag, Action::Drag, Action::CancelledDrag, Action::Undo, Action::Undo, Action::Redo,
    Action::Zoom, Action::SetAlerts};

using Row = std::tuple<quint64, QString, double, double, double, double>;

std::vector<Row> snapshot(Editor &editor)
{
    const StandModel *model = editor.stands();
    std::vector<Row> rows;
    for (int row = 0; row < model->rowCount(); ++row) {
        const QModelIndex index = model->index(row);
        rows.emplace_back(model->data(index, StandModel::StandIdRole).toULongLong(),
            model->data(index, StandModel::LabelRole).toString(),
            model->data(index, StandModel::XmRole).toDouble(),
            model->data(index, StandModel::YmRole).toDouble(),
            model->data(index, StandModel::WidthMRole).toDouble(),
            model->data(index, StandModel::HeightMRole).toDouble());
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
        std::uniform_int_distribution<std::size_t> pickAction(0, kActions.size() - 1);
        std::uniform_real_distribution<double> pixel(kMinPixel, kMaxPixel);
        std::uniform_int_distribution<int> wheelNotches(-kMaxWheelNotches, kMaxWheelNotches);
        std::bernoulli_distribution raiseAlert(kAlertChance);
        const auto randomPoint = [&] { return QPointF(pixel(rng), pixel(rng)); };

        Editor editor;
        QAbstractItemModelTester const standsTester(
            editor.stands(), QAbstractItemModelTester::FailureReportingMode::QtTest);
        QAbstractItemModelTester const alertsTester(
            editor.alerts(), QAbstractItemModelTester::FailureReportingMode::QtTest);

        for (int step = 0; step < kSteps; ++step) {
            switch (kActions.at(pickAction(rng))) {
            case Action::UseStandTool:
                editor.setActiveTool(Editor::ToolKind::Stand);
                break;
            case Action::UseSelectTool:
                editor.setActiveTool(Editor::ToolKind::Select);
                break;
            case Action::Drag: {
                const QPointF from = randomPoint();
                const QPointF target = randomPoint();
                editor.press(from);
                for (int move = 1; move <= kDragMoves; ++move) {
                    editor.move(from + (target - from) * move / kDragMoves);
                }
                editor.release(target);
                break;
            }
            case Action::CancelledDrag:
                editor.press(randomPoint());
                editor.move(randomPoint());
                editor.cancel();
                break;
            case Action::Undo:
                editor.undo();
                break;
            case Action::Redo:
                editor.redo();
                break;
            case Action::Zoom:
                editor.view()->wheelZoomAt(randomPoint(), wheelNotches(rng) * kWheelNotch);
                break;
            case Action::SetAlerts: {
                std::vector<mangrove::core::Alert> alerts;
                const StandModel *stands = editor.stands();
                for (int row = 0; row < stands->rowCount(); ++row) {
                    if (raiseAlert(rng)) {
                        alerts.push_back({"overlap",
                            {mangrove::core::StandId{
                                stands->data(stands->index(row), StandModel::StandIdRole)
                                    .toULongLong()}}});
                    }
                }
                editor.setAlerts(alerts);
                break;
            }
            }
            QCOMPARE(editor.alerts()->count(), editor.alerts()->rowCount());
            if (QTest::currentTestFailed()) {
                return;
            }
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

        for (int redone = 0; redone < undone; ++redone) {
            editor.redo();
        }
        QVERIFY(snapshot(editor) == finalRows);
        QCOMPARE(editor.alerts()->count(), finalAlerts);
    }
};

QTEST_GUILESS_MAIN(TestUndoInvariants)
#include "tst_undo_invariants.moc"
