/*
// Mangrove Project
// tst_ui.cpp
// Description: End-to-end tests. Loads the real qml/Main.qml with the static
//              QML modules, sends real mouse/keyboard events to the window and
//              checks the result. Any QML warning fails the test.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "Editor.hpp"
#include "Translation.hpp"

#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <QtQml/qqmlextensionplugin.h>
#include <QtTest>

#include <algorithm>
#include <memory>

Q_IMPORT_QML_PLUGIN(Mangrove_AppPlugin)
Q_IMPORT_QML_PLUGIN(Mangrove_ThemePlugin)
Q_IMPORT_QML_PLUGIN(Mangrove_ControlsPlugin)
Q_IMPORT_QML_PLUGIN(Mangrove_ShellPlugin)
Q_IMPORT_QML_PLUGIN(Mangrove_PanelsPlugin)

using namespace mangrove::app;

class TestUi : public QObject {
    Q_OBJECT

private slots:
    // Each test gets its own engine, hence its own Editor singleton. Shortcuts need an
    // active window.
    void init()
    {
        m_engine = std::make_unique<QQmlApplicationEngine>();
        QObject::connect(
            m_engine.get(), &QQmlEngine::warnings, this, [](const QList<QQmlError> &list) {
                for (const QQmlError &error : list)
                    QTest::qFail(qPrintable(error.toString()), __FILE__, __LINE__);
            });
        m_engine->load(QUrl::fromLocalFile(QStringLiteral(MANGROVE_MAIN_QML)));
        QCOMPARE(m_engine->rootObjects().size(), 1);

        m_window = qobject_cast<QQuickWindow *>(m_engine->rootObjects().first());
        QVERIFY(m_window);
        QVERIFY(QTest::qWaitForWindowExposed(m_window));
        m_editor = m_engine->singletonInstance<Editor *>("Mangrove.App", "Editor");
        QVERIFY(m_editor);

        m_window->requestActivate();
        QVERIFY(QTest::qWaitForWindowActive(m_window));
    }

    void cleanup()
    {
        m_engine.reset();
        m_window = nullptr;
        m_editor = nullptr;
    }

    void barsSurroundTheWorkspace()
    {
        verifyBarsSurroundWorkspace();
    }

    // When the window shrinks, only the workspace shrinks: the bars keep their thickness.
    void barsKeepTheirThicknessWhenWindowResizes()
    {
        const qreal topHeight = item("topBar")->height();
        const qreal statusHeight = item("statusBar")->height();
        const qreal toolbarWidth = item("toolbar")->width();
        const qreal panelWidth = item("alertsPanel")->width();
        const qreal workspaceWidth = item("workspace")->width();

        m_window->resize(m_window->width() - 200, m_window->height() - 100);
        QTRY_COMPARE(item("topBar")->width(), qreal(m_window->width()));

        QCOMPARE(item("topBar")->height(), topHeight);
        QCOMPARE(item("statusBar")->height(), statusHeight);
        QCOMPARE(item("toolbar")->width(), toolbarWidth);
        QCOMPARE(item("alertsPanel")->width(), panelWidth);
        QCOMPARE(item("workspace")->width(), workspaceWidth - 200);
        verifyBarsSurroundWorkspace();
    }

    void startsWithSelectToolAndEmptyHistory()
    {
        QCOMPARE(m_editor->activeTool(), Editor::ToolKind::Select);
        QVERIFY(item("tool_select")->property("active").toBool());
        QVERIFY(!item("tool_stand")->property("active").toBool());
        QVERIFY(!item("undoButton")->isEnabled());
        QVERIFY(!item("redoButton")->isEnabled());
        QCOMPARE(m_editor->alerts()->count(), 0);
    }

    void toolButtonsSwitchTools()
    {
        click(item("tool_stand"));
        QCOMPARE(m_editor->activeTool(), Editor::ToolKind::Stand);
        QVERIFY(item("tool_stand")->property("active").toBool());
        QVERIFY(!item("tool_select")->property("active").toBool());

        click(item("tool_select"));
        QCOMPARE(m_editor->activeTool(), Editor::ToolKind::Select);
    }

    void keyboardShortcutsSwitchTools()
    {
        QTest::keyClick(m_window, Qt::Key_R);
        QCOMPARE(m_editor->activeTool(), Editor::ToolKind::Stand);
        QTest::keyClick(m_window, Qt::Key_V);
        QCOMPARE(m_editor->activeTool(), Editor::ToolKind::Select);
    }

    void statusBarShowsActiveTool()
    {
        QQuickItem *label = item("activeToolLabel");
        QCOMPARE(label->property("text").toString(), QStringLiteral("Select"));
        QTest::keyClick(m_window, Qt::Key_R);
        QCOMPARE(label->property("text").toString(), QStringLiteral("Stand"));
    }

    void undoRedoButtonsFollowHistory()
    {
        addStandThroughEditor();
        QVERIFY(item("undoButton")->isEnabled());
        QVERIFY(!item("redoButton")->isEnabled());

        click(item("undoButton"));
        QCOMPARE(m_editor->stands()->rowCount(), 0);
        QVERIFY(!item("undoButton")->isEnabled());
        QVERIFY(item("redoButton")->isEnabled());

        click(item("redoButton"));
        QCOMPARE(m_editor->stands()->rowCount(), 1);
    }

    void undoRedoShortcuts()
    {
        addStandThroughEditor();
        QTest::keyClick(m_window, Qt::Key_Z, Qt::ControlModifier);
        QCOMPARE(m_editor->stands()->rowCount(), 0);
        QTest::keyClick(m_window, Qt::Key_Z, Qt::ControlModifier | Qt::ShiftModifier);
        QCOMPARE(m_editor->stands()->rowCount(), 1);
        QTest::keyClick(m_window, Qt::Key_Z, Qt::ControlModifier);
        QTest::keyClick(m_window, Qt::Key_Y, Qt::ControlModifier);
        QCOMPARE(m_editor->stands()->rowCount(), 1);
    }

    void escapeCancelsGesture()
    {
        m_editor->setActiveTool(Editor::ToolKind::Stand);
        m_editor->press({160, 160});
        m_editor->move({320, 320});
        QVERIFY(m_editor->hasPreview());
        QTest::keyClick(m_window, Qt::Key_Escape);
        QVERIFY(!m_editor->hasPreview());
    }

    void alertShowsInAlertsPanel()
    {
        QQuickItem *panel = item("alertsPanel");
        QVERIFY(textShown(panel, QStringLiteral("No issues on this plan.")));

        mangrove::core::Plan plan;
        const mangrove::core::Stand a = plan.makeStand({10, 10, 4, 3});
        plan.insertStand(0, a);
        const mangrove::core::Stand b = plan.makeStand({12, 11, 4, 3});
        plan.insertStand(1, b);
        m_editor->alerts()->setAlerts({{"overlap", {a.id, b.id}}}, plan);
        QCOMPARE(m_editor->alerts()->count(), 1);

        QTRY_VERIFY(textShown(panel, QStringLiteral("Stands S01 and S02 overlap.")));
        QVERIFY(!textShown(panel, QStringLiteral("No issues on this plan.")));
    }

    void alertsShowInWarningColor()
    {
        QObject *theme = m_engine->singletonInstance<QObject *>("Mangrove.Theme", "Theme");
        QVERIFY(theme);
        const QColor warning = theme->property("warning").value<QColor>();
        QQuickItem *badge = item("countBadge");
        QVERIFY(badge->property("color").value<QColor>() != warning);

        addStandThroughEditor();
        const StandModel *stands = m_editor->stands();
        const mangrove::core::StandId stand{
            stands->data(stands->index(0), StandModel::StandIdRole).toULongLong()};
        m_editor->setAlerts({{"inside-hall", {stand}}});
        QTRY_COMPARE(badge->property("color").value<QColor>(), warning);
        QVERIFY(textShown(item("alertsPanel"), QStringLiteral("Stand S01 is outside the hall.")));

        m_editor->setAlerts({});
        QTRY_VERIFY(badge->property("color").value<QColor>() != warning);
        QVERIFY(textShown(item("alertsPanel"), QStringLiteral("No issues on this plan.")));
    }

    // The other tests run without a translator, so they read the English source texts.
    // Texts built in C++ with tr() are translated too. The translator uninstalls itself
    // at the end of the test.
    void interfaceIsInFrench()
    {
        const auto translator = loadTranslation();
        QVERIFY(translator);
        QCoreApplication::installTranslator(translator.get());
        m_engine->retranslate();

        QCOMPARE(item("activeToolLabel")->property("text").toString(), QStringLiteral("Sélection"));
        QCOMPARE(
            item("undoButton")->property("tooltip").toString(), QStringLiteral("Annuler (Ctrl+Z)"));
        QTRY_VERIFY(textShown(item("alertsPanel"), QStringLiteral("Aucun problème sur ce plan.")));

        mangrove::core::Plan plan;
        const mangrove::core::Stand a = plan.makeStand({10, 10, 4, 3});
        plan.insertStand(0, a);
        const mangrove::core::Stand b = plan.makeStand({12, 11, 4, 3});
        plan.insertStand(1, b);
        m_editor->alerts()->setAlerts({{"overlap", {a.id, b.id}}}, plan);
        QTRY_VERIFY(textShown(
            item("alertsPanel"), QStringLiteral("Les stands S01 et S02 se chevauchent.")));
    }

private:
    // Searches the visual tree: Repeater delegates are not QObject children of the window.
    QQuickItem *item(const char *objectName) const
    {
        QQuickItem *found = findItem(m_window->contentItem(), QLatin1String(objectName));
        if (!found)
            qFatal("No item named \"%s\" in Main.qml", objectName);
        return found;
    }

    static QQuickItem *findItem(QQuickItem *root, QLatin1String objectName)
    {
        if (root->objectName() == objectName)
            return root;
        const auto children = root->childItems();
        for (QQuickItem *child : children) {
            if (QQuickItem *found = findItem(child, objectName))
                return found;
        }
        return nullptr;
    }

    static QRectF sceneRect(const QQuickItem *item)
    {
        return item->mapRectToScene(QRectF(0, 0, item->width(), item->height()));
    }

    // Top and status bars span the window; toolbar, workspace and panel sit side
    // by side in between; nothing overlaps and the workspace is not empty.
    void verifyBarsSurroundWorkspace() const
    {
        const QRectF top = sceneRect(item("topBar"));
        const QRectF status = sceneRect(item("statusBar"));
        const QRectF tools = sceneRect(item("toolbar"));
        const QRectF panel = sceneRect(item("alertsPanel"));
        const QRectF workspace = sceneRect(item("workspace"));

        QCOMPARE(top.topLeft(), QPointF(0, 0));
        QCOMPARE(top.width(), qreal(m_window->width()));
        QCOMPARE(status.bottom(), qreal(m_window->height()));
        QCOMPARE(status.width(), qreal(m_window->width()));

        QCOMPARE(tools.left(), 0.0);
        QCOMPARE(tools.right(), workspace.left());
        QCOMPARE(panel.left(), workspace.right());
        QCOMPARE(panel.right(), qreal(m_window->width()));
        for (const QRectF &middle : {tools, workspace, panel}) {
            QCOMPARE(middle.top(), top.bottom());
            QCOMPARE(middle.bottom(), status.top());
        }
        QVERIFY(workspace.width() > 0 && workspace.height() > 0);
    }

    void click(QQuickItem *target)
    {
        const QPointF center(target->width() / 2, target->height() / 2);
        QTest::mouseClick(m_window, Qt::LeftButton, {}, target->mapToScene(center).toPoint());
    }

    // An edit made through the editor, as the plan canvas will make it.
    void addStandThroughEditor()
    {
        m_editor->setActiveTool(Editor::ToolKind::Stand);
        m_editor->press({160, 160});
        m_editor->release({160, 160});
        QCOMPARE(m_editor->stands()->rowCount(), 1);
    }

    static bool textShown(QQuickItem *root, const QString &text)
    {
        if (!root->isVisible())
            return false;
        if (root->property("text").toString() == text)
            return true;
        const auto children = root->childItems();
        return std::any_of(children.begin(), children.end(),
            [&](QQuickItem *child) { return textShown(child, text); });
    }

    std::unique_ptr<QQmlApplicationEngine> m_engine;
    QQuickWindow *m_window = nullptr;
    Editor *m_editor = nullptr;
};

QTEST_MAIN(TestUi)
#include "tst_ui.moc"
