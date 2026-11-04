# EditorActions

> QML · module `Mangrove.Shell` — [`EditorActions.qml`](../../../../qml/Mangrove/Shell/EditorActions.qml)

Singleton, registry of every editor command. Bars show them and `KeyboardShortcuts` binds their keys: a command is declared here only.

## Contents

| Property | Action | Shortcuts | Note |
|---|---|---|---|
| `tools[0]` | Select | V | [ToolAction](ToolAction.md) |
| `tools[1]` | Stand | R | [ToolAction](ToolAction.md) |
| `activeTool` | — | — | read-only: the active tool |
| `undo` | Undo | Ctrl+Z | enabled when `Editor.canUndo` |
| `redo` | Redo | Ctrl+Shift+Z, Ctrl+Y | enabled when `Editor.canRedo` |
| `cancel` | Cancel | Escape | no button |
| `commands` | — | — | `[undo, redo, cancel]`, for the shortcuts |

## Extending it

- **A tool**: a `ToolAction` in `tools`. It shows up in the toolbar on its own and its shortcut
  works (C++ side: see [ATool](../../../src/app/tools/ATool.md)).
- **A command**: a `readonly property AppAction`, added to `commands` for its shortcut, plus an
  `ActionButton` in the bar you want if it has a button. Step by step:
  [guides/QML_BUTTON.md](../../../guides/QML_BUTTON.md).

---
[↑ Contents](../../../README.md)
