# Adding a button on the QML side

This guide explains how to add a button (and its keyboard shortcut) **without touching any C++**:
the action it triggers already exists in `Editor`, or the button only changes what is displayed.

If the button must do something `Editor` cannot do yet (change the plan, read a new piece of
state…), follow [APP_BUTTON.md](APP_BUTTON.md) instead: it uses this guide for the QML part.

## 1. How it works

A command is declared **once**, as an `AppAction` in
[`EditorActions.qml`](../../qml/Mangrove/Shell/EditorActions.qml). Then:

```
EditorActions.qml: AppAction { icon, text, shortcuts, enabled, onTriggered }
        │
        ├── ActionButton { appAction: EditorActions.x }  placed in a bar   → the button
        └── the `commands` list, read by KeyboardShortcuts                 → the shortcut
```

The button holds no logic: its icon, tooltip, state (enabled or greyed out) and click all come from
the action. The same `AppAction` can therefore be shown in several bars without duplicating code.

| File | Role |
|---|---|
| [`Theme/Icons.qml`](../../qml/Mangrove/Theme/Icons.qml) | the icon's SVG path |
| [`Shell/EditorActions.qml`](../../qml/Mangrove/Shell/EditorActions.qml) | the action's declaration |
| [`Shell/TopBar.qml`](../../qml/Mangrove/Shell/TopBar.qml), [`Toolbar.qml`](../../qml/Mangrove/Shell/Toolbar.qml), [`StatusBar.qml`](../../qml/Mangrove/Shell/StatusBar.qml) | where the button appears |
| [`tests/ui/tst_ui.cpp`](../../tests/ui/tst_ui.cpp) | the test of the button and its shortcut |

## 2. A real example: the Undo button

The icon, in `Icons.qml`:

```qml
"undo": "M9 14 L4 9 L9 4 M4 9 H14.5 A5.5 5.5 0 0 1 14.5 20 H11",
```

The action, in `EditorActions.qml`, added to the `commands` list:

```qml
readonly property AppAction undo: AppAction {
    key: "undo"
    iconName: "undo"
    text: qsTr("Undo")
    shortcuts: ["Ctrl+Z"]
    enabled: Editor.canUndo      // greyed out when the history is empty
    onTriggered: Editor.undo()   // the work is done in C++
}

readonly property list<AppAction> commands: [undo, redo, cancel]
```

The button, in `TopBar.qml`:

```qml
trailing: [
    ActionButton {
        objectName: "undoButton"
        size: Theme.barButtonSize
        appAction: EditorActions.undo
    },
    // ...
]
```

That is all: the Ctrl+Z shortcut, the "Undo (Ctrl+Z)" tooltip and the greyed-out state are
handled automatically.

## 3. The recipe

### 3.1 The icon

Add an entry to `Icons.qml`:

```qml
"myIcon": "M4 7 H20 M10 11 V16 ..."
```

- It is the `d` attribute of an SVG `<path>`, on a **24 × 24** grid.
- The icon is **stroked only** (no fill): a filled shape only shows its outline. Use lines (`M`,
  `L`, `H`, `V`), arcs (`A`) and `Z`.
- The easiest way: draw it in Inkscape or Figma on a 24 × 24 page with a 1.5 px stroke, then copy
  the `d` attribute.

### 3.2 The action

In `EditorActions.qml`, add a property **and** add it to `commands`:

```qml
readonly property AppAction myAction: AppAction {
    key: "myAction"               // stable, unique name
    iconName: "myIcon"            // key of Icons.paths
    text: qsTr("My action")       // tooltip; always inside qsTr()
    shortcuts: ["Ctrl+K"]         // optional; the first one is shown in the tooltip
    enabled: Editor.canDoIt       // optional; an Editor property that has a NOTIFY signal
    onTriggered: Editor.doIt()    // a call to Editor, nothing more
}

readonly property list<AppAction> commands: [undo, redo, myAction, cancel]
```

| `AppAction` property | Used for |
|---|---|
| `key` | identifying the action (object names, tests) |
| `iconName` | picking the icon in `Icons.paths` |
| `text` | the tooltip, followed by the first shortcut in parentheses |
| `shortcuts` | the key sequences (`"Ctrl+Z"`, `"Delete"`, `"Escape"`…) |
| `enabled` | greying out the button **and** disabling the shortcut |
| `active` | showing the button as pressed (a mode, such as the current tool) |
| `onTriggered` | what the action does |

Rules:

- **No logic in `onTriggered`** (rule 1 of the [README](../../README.md)): computations, conditions
  and loops belong in `Editor`, in C++.
- Every text shown to the user is written in English inside `qsTr()`, then translated into French
  in `translations/mangrove_fr.ts` (see [TRANSLATIONS.md](TRANSLATIONS.md)). Never French in the
  code.
- To fire an action from QML, call `trigger()`, never `triggered()`: `trigger()` does nothing while
  the action is disabled.
- **An editing tool** is not an `AppAction` but a `ToolAction` in the `tools` list. It shows up in
  the toolbar on its own, but it needs C++: see the "A tool" row of the "Where do I add X?" table in the
  [README](../../README.md).

### 3.3 The button

Place an `ActionButton` in the bar you want, with an `objectName` (tests rely on it):

```qml
ActionButton {
    objectName: "myActionButton"
    size: Theme.barButtonSize
    appAction: EditorActions.myAction
}
```

| Bar | Where to put the button |
|---|---|
| `TopBar.qml` (top) | declared children go on the left (`leading`), items in `trailing: [...]` on the right. Size: `Theme.barButtonSize`. |
| `Toolbar.qml` (left) | nothing to do: it shows every `ToolAction` of `EditorActions.tools`. |
| `StatusBar.qml` (bottom) | for information only (`TextLabel`), no buttons for now. |
| `qml/Mangrove/Panels/` | inside the content of a panel (`SidePanel`). |

If you create a **new `.qml` file**, add it to the `QML_FILES` of its module's `CMakeLists.txt`,
otherwise it cannot be found.

### 3.4 A plain button, without an action

A button that only changes local display (collapsing a section of a panel, for example) is not a
command: it has no shortcut and no place in `EditorActions`. Use `IconToolButton` directly, the
building block `ActionButton` is made of. For example, in a panel that has a `collapsed` property
(the `chevron` icon would have to be added):

```qml
IconToolButton {
    objectName: "collapseButton"
    iconName: "chevron"
    tooltip: qsTr("Collapse")
    active: root.collapsed
    onClicked: root.collapsed = !root.collapsed
}
```

As soon as the button calls `Editor` or changes the plan, it is a command: go back to an
`AppAction`. Never use a Qt Quick Controls `Button` with hand-written colors: colors, sizes and
radii come from `Theme.qml` (rule 6 of the README).

## 4. The test

In [`tests/ui/tst_ui.cpp`](../../tests/ui/tst_ui.cpp), test the button **and** the shortcut. Use
`undoRedoButtonsFollowHistory` as a model:

```cpp
void myActionButtonDoesIt()
{
    QVERIFY(!item("myActionButton")->isEnabled()); // greyed out at start
    addStandThroughEditor();                       // a state where the action is possible
    QVERIFY(item("myActionButton")->isEnabled());

    click(item("myActionButton"));
    QCOMPARE(m_editor->stands()->rowCount(), /* expected result */);
}

void myActionShortcut()
{
    addStandThroughEditor();
    QTest::keyClick(m_window, Qt::Key_K, Qt::ControlModifier);
    QCOMPARE(m_editor->stands()->rowCount(), /* expected result */);
}
```

- `item("…")` finds an item by its `objectName`.
- Any QML warning fails the test, so a typo in a property name is caught right away.
- These tests run without a translator: compare the **English** texts (`"Undo"`).
- Run: `ctest --test-dir build -L ui` (details in [TESTING.md](TESTING.md)).

## 5. Documentation

Update the pages in [`Documentation/`](../README.md):

- `qml/Mangrove/Shell/EditorActions.md`: one row in the "Contents" table;
- the bar's page (for example `qml/Mangrove/Shell/TopBar.md`);
- `qml/Mangrove/Theme/Icons.md`: the list of available icons.

## 6. Common problems

| Symptom | Cause |
|---|---|
| The button works, the shortcut does not | the action is not in the `commands` list |
| Two actions, one shortcut, nothing happens | two actions use the same key: Qt fires neither of them |
| The tooltip is in English in the app | the text is not translated: run `update_translations`, then translate it in `mangrove_fr.ts` |
| The button is empty | `iconName` matches no key of `Icons.paths` |
| The button stays greyed out | `appAction` is not set, or `enabled` is bound to a property without `NOTIFY` |
| "X is not a type" at build time or at launch | the new `.qml` file is not in the module's `CMakeLists.txt` |

## 7. Checklist

- [ ] The icon is in `Icons.qml` (stroke only, on a 24 × 24 grid).
- [ ] The `AppAction` is in `EditorActions.qml` **and** in `commands`.
- [ ] The `ActionButton` has an `objectName`.
- [ ] No hard-coded values (everything comes from `Theme`), texts in English inside `qsTr()`.
- [ ] `update_translations` run and the new texts translated in `translations/mangrove_fr.ts`.
- [ ] One test for the click and one for the shortcut in `tst_ui`.
- [ ] The `Documentation/` pages are up to date.
- [ ] `cmake --build build && ctest --test-dir build && cmake --build build --target all_qmllint`
      pass with no errors or warnings.
