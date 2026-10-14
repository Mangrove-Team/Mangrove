# Adding a button that acts on the application

This guide explains how to add a button that does something **new**: change the plan, read a piece
of state that `Editor` does not expose yet… This means writing C++ before the QML.

If `Editor` can already do what the button triggers, only the QML part is needed: see
[QML_BUTTON.md](QML_BUTTON.md).

The guide follows a complete example: **a "Delete" button that removes the selected stand**
(Delete and Backspace keys), undoable with Ctrl+Z. This code compiles and passes the whole test
suite, but it is not in the repository yet: it is a model to follow.

## 1. The path of a click

```
click on the button, or the Delete key
  → AppAction.onTriggered                      qml/Mangrove/Shell/EditorActions.qml
  → Editor.deleteSelection()                   src/app/Editor.cpp        (Q_INVOKABLE)
  → RemoveStandCommand pushed onto the QUndoStack
                                               src/app/commands/remove/
  → PlanDocument changes the Plan and emits its signals
  → StandModel and Editor.hasSelection update
  → QML redraws on its own (bindings)
```

Three rules from the [README](../../README.md) drive this split:

- **No logic in QML**: the button calls `Editor`, nothing more.
- **Every change to the plan goes through a `QUndoCommand`**: that is what makes it undoable.
- **`Editor` is the only entry point from QML**: neither `PlanDocument` nor `Plan` is exposed.

## 2. The steps

| # | What | Where |
|---|---|---|
| 1 | The undoable command | `src/app/commands/remove/` + `src/app/CMakeLists.txt` |
| 2 | The method and state exposed to QML | `src/app/Editor.{hpp,cpp}` |
| 3 | The icon, the action, the button and the translations | `Icons.qml`, `EditorActions.qml`, `TopBar.qml`, `translations/mangrove_fr.ts` |
| 4 | The tests | `tests/app/` and `tests/ui/` |
| 5 | The documentation | `Documentation/` |

If the button does not change the plan (it only reads some state, or changes the view), skip
step 1.

## 3. Step 1: the undoable command

A command knows how to do **and** undo. It stores everything it needs to undo **as soon as it is
constructed**: here, the whole stand and its position in the drawing order, so that it comes back
exactly where it was (below or above the others).

`src/app/commands/remove/RemoveStandCommand.hpp`:

```cpp
/*
// Mangrove Project
// RemoveStandCommand.hpp
// Description: Removes a stand from the plan. Undo puts it back at the same place in the drawing order.
// Copyright (c) Mangrove. All rights reserved.
*/

#pragma once

#include "commands/APlanCommand.hpp"

#include <QCoreApplication>

#include <cstddef>

namespace mangrove::app {

    class RemoveStandCommand final : public APlanCommand {

            Q_DECLARE_TR_FUNCTIONS(RemoveStandCommand)

        public:
            // `index` is the stand's position in the drawing order, restored by undo.
            RemoveStandCommand(PlanDocument &document, const core::Stand &stand, std::size_t index);

            void redo() override;
            void undo() override;

        private:
            core::Stand _stand;
            std::size_t _index;
    }; // class RemoveStandCommand

} // namespace mangrove::app
```

`src/app/commands/remove/RemoveStandCommand.cpp`:

```cpp
/*
// Mangrove Project
// RemoveStandCommand.cpp
// Description: Removes a stand from the plan.
// Copyright (c) Mangrove. All rights reserved.
*/

#include "commands/remove/RemoveStandCommand.hpp"

#include "Conversions.hpp"

namespace mangrove::app {

    RemoveStandCommand::RemoveStandCommand(PlanDocument &document, const core::Stand &stand,
                                           std::size_t index)
        : APlanCommand(document), _stand(stand), _index(index)
    {
        setText(tr("Remove stand %1").arg(toQt(_stand.label)));
    }

    void RemoveStandCommand::redo()
    {
        removeStand(_stand.id);
    }

    void RemoveStandCommand::undo()
    {
        insertStand(_index, _stand);
    }

} // namespace mangrove::app
```

Then list the files in `src/app/CMakeLists.txt`, after the other commands:

```cmake
        commands/move/MoveStandCommand.hpp commands/move/MoveStandCommand.cpp
        commands/remove/RemoveStandCommand.hpp commands/remove/RemoveStandCommand.cpp
```

Good to know:

- A command derives from `APlanCommand`, which gives it `plan()`, `insertStand()`, `removeStand()`
  and `setStandRect()`. If the change you need does not exist, add it as a **private** method of
  `PlanDocument`, then an accessor in `APlanCommand` (see the "Where do I add X?" table in the README).
- `setText()` sets the step's name in the history; it goes through `tr()` and `arg()`, in English.
  `Q_DECLARE_TR_FUNCTIONS` is what gives the class its `tr()`.
- If several consecutive commands must merge into a single step (like a drag), give them an id in
  `CommandIds.hpp` and implement `id()` and `mergeWith()`: see `MoveStandCommand`.

## 4. Step 2: the Editor

QML needs two things: **a method** to call and **a piece of state** to grey out the button when
the action is not possible.

In `src/app/Editor.hpp`:

```cpp
            Q_PROPERTY(bool canRedo READ canRedo NOTIFY canRedoChanged)
            Q_PROPERTY(bool hasSelection READ hasSelection NOTIFY selectionChanged)   // new

            // ...
            [[nodiscard]] bool canRedo() const;
            [[nodiscard]] bool hasSelection() const noexcept { return static_cast<bool>(_selected); }

            // ...
            Q_INVOKABLE void redo();
            // Removes the selected stand, as one undo step. Does nothing without a selection.
            Q_INVOKABLE void deleteSelection();

        signals:
            // ...
            void canRedoChanged();
            void selectionChanged();
```

In `src/app/Editor.cpp`:

```cpp
#include "commands/remove/RemoveStandCommand.hpp"

    void Editor::deleteSelection()
    {
        cancel(); // a stand being dragged goes back to its place first
        const core::Plan &current = _document.plan();
        const core::Stand *stand = current.find(_selected);
        if (!stand)
            return;
        _document.undoStack()->push(
            new RemoveStandCommand(_document, *stand, *current.indexOf(_selected)));
    }
```

And so that QML is notified when the selection changes, `selectStand()` emits the signal:

```cpp
    void Editor::selectStand(core::StandId id)
    {
        if (id == _selected)
            return;
        _selected = id;
        _stands.setSelectedStand(id);
        emit selectionChanged();
    }
```

Good to know:

- **QML can call the method at any time**, even while the button is greyed out (through a
  shortcut, a script, a test). The method must therefore check on its own that the action is
  possible, and do nothing otherwise.
- **Never push a command that cannot apply**: the history would contain a step that does nothing.
- **Cancel the current gesture first** with `cancel()`, as `undo()` and `redo()` do: otherwise a
  stand being dragged would be deleted at its temporary position.
- **Every property read by QML has a `NOTIFY` signal**, emitted on every change. Without it, the
  button never becomes enabled again.
- A property exposed to QML is declared with `Q_PROPERTY`; a method with `Q_INVOKABLE`. Never use
  `setContextProperty` (rule 7 of the README).

## 5. Step 3: the QML

This is exactly the recipe from [QML_BUTTON.md](QML_BUTTON.md). For the example:

`qml/Mangrove/Theme/Icons.qml`, a trash can:

```qml
"delete": "M4 7 H20 M9.5 7 V4.5 H14.5 V7 M6.5 7 L7.5 20 H16.5 L17.5 7 M10 11 V16 M14 11 V16"
```

`qml/Mangrove/Shell/EditorActions.qml`:

```qml
readonly property AppAction deleteSelection: AppAction {
    key: "delete"
    iconName: "delete"
    text: qsTr("Delete")
    shortcuts: ["Delete", "Backspace"]
    enabled: Editor.hasSelection
    onTriggered: Editor.deleteSelection()
}

// Every action that is not a tool.
readonly property list<AppAction> commands: [undo, redo, deleteSelection, cancel]
```

`qml/Mangrove/Shell/TopBar.qml`, before Undo / Redo:

```qml
trailing: [
    ActionButton {
        objectName: "deleteButton"
        size: Theme.barButtonSize
        appAction: EditorActions.deleteSelection
    },
    ActionButton {
        objectName: "undoButton"
        // ...
```

Then add the French translations of the new texts ("Delete", "Remove stand %1"):

```sh
cmake --build build --target update_translations
```

and translate them in `translations/mangrove_fr.ts`, for example `Supprimer` and
`Supprimer le stand %1` (see [TRANSLATIONS.md](TRANSLATIONS.md)).

## 6. Step 4: the tests

The "Which test for which change?" table in [TESTING.md](TESTING.md) gives the minimum. For a
command triggered by a button, that means four places.

**The normal scenario**, in `tests/app/tst_editor.cpp`: do, undo, redo.

```cpp
void deleteRemovesSelectedStandAsOneUndoStep()
{
    Editor editor;
    editor.setActiveTool(Editor::ToolKind::Stand);
    drag(editor, px(10, 10), px(10, 10), 0);
    drag(editor, px(30, 30), px(30, 30), 0);
    editor.setActiveTool(Editor::ToolKind::Select);
    drag(editor, px(11, 11), px(11, 11), 0); // selects the first stand
    QVERIFY(editor.hasSelection());

    editor.deleteSelection();
    QCOMPARE(editor.stands()->rowCount(), 1);
    QVERIFY(!editor.hasSelection());

    editor.undo();
    QCOMPARE(editor.stands()->rowCount(), 2);
    QCOMPARE(standRect(editor, 0), QRectF(10, 10, 4, 3)); // back under the other one
    editor.redo();
    QCOMPARE(editor.stands()->rowCount(), 1);
}
```

**Nonsensical calls**, in `tests/app/tst_app_robustness.cpp`: no selection, mid-gesture.

```cpp
void deleteWithoutSelectionIsANoOp()
{
    Editor editor;
    editor.deleteSelection();
    QVERIFY(!editor.canUndo());
}

void deleteMidDragPutsStandBackFirst()
{
    Editor editor;
    editor.setActiveTool(Editor::ToolKind::Stand);
    click(editor, px(10, 10));
    editor.setActiveTool(Editor::ToolKind::Select);
    editor.press(px(11, 11));
    editor.move(px(25, 25));
    editor.deleteSelection();
    editor.release(px(25, 25)); // gesture already cancelled
    QCOMPARE(editor.stands()->rowCount(), 0);

    editor.undo();
    const QModelIndex first = editor.stands()->index(0);
    QCOMPARE(editor.stands()->data(first, StandModel::XmRole).toDouble(), 10.0);
}
```

**Random sessions**, in `tests/app/tst_undo_invariants.cpp`: add the action to the draw. The test
then checks that undoing everything and redoing everything gives back the same plan.

```cpp
std::uniform_int_distribution<int> action(0, 10); // was (0, 9)
// ...
case 10:
    editor.deleteSelection();
    break;
```

**The real button and the real shortcut**, in `tests/ui/tst_ui.cpp`:

```cpp
void deleteButtonFollowsSelection()
{
    QVERIFY(!item("deleteButton")->isEnabled());
    addStandThroughEditor(); // the new stand is selected
    QVERIFY(item("deleteButton")->isEnabled());

    click(item("deleteButton"));
    QCOMPARE(m_editor->stands()->rowCount(), 0);
    QVERIFY(!item("deleteButton")->isEnabled());
}

void deleteShortcut()
{
    addStandThroughEditor();
    QTest::keyClick(m_window, Qt::Key_Delete);
    QCOMPARE(m_editor->stands()->rowCount(), 0);
}
```

## 7. Step 5: the documentation

In [`Documentation/`](../README.md):

- **New page** `src/app/commands/remove/RemoveStandCommand.md` (based on `AddStandCommand.md`),
  plus its line in the table of contents of `Documentation/README.md`.
- `src/app/Editor.md`: `hasSelection` and `deleteSelection()` in the "API exposed to QML" tables.
- `qml/Mangrove/Shell/EditorActions.md`, `qml/Mangrove/Shell/TopBar.md`,
  `qml/Mangrove/Theme/Icons.md`.
- The [README](../../README.md): the `Command` row of the "Interfaces and abstract classes" table
  lists the concrete commands.

## 8. Checklist

- [ ] The command derives from `APlanCommand`, stores everything needed to undo when it is
      constructed, and its files are in `src/app/CMakeLists.txt`.
- [ ] The `Q_INVOKABLE` method checks that the action is possible and pushes nothing otherwise.
- [ ] The current gesture is cancelled first (`cancel()`).
- [ ] The state that greys out the button is a `Q_PROPERTY` with a `NOTIFY` signal emitted on every
      change.
- [ ] On the QML side: icon, `AppAction` in `commands`, `ActionButton` with an `objectName`.
- [ ] New texts in English in the code and translated in `translations/mangrove_fr.ts`.
- [ ] Tests: `tst_editor`, `tst_app_robustness`, `tst_undo_invariants`, `tst_ui`, `ctest -L i18n`.
- [ ] `Documentation/` pages created or updated.
- [ ] Everything passes with no warnings:
      `cmake --build build && ctest --test-dir build && cmake --build build --target all_qmllint`,
      then the suite under `linux-asan` (security checklist in [TESTING.md](TESTING.md#7-security-what-we-protect-and-the-checklist)).
