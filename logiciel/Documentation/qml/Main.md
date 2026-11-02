# Main

> QML · module `Mangrove.Main` — [`Main.qml`](../../qml/Main.qml)

Main window. It only says which bar goes on which edge; everything else lives in the modules.

## Contents

| Item | Role |
|---|---|
| `Window` | default and minimum size, title, background color (all from [Theme](Mangrove/Theme/Theme.md)) |
| `DockLayout` | `topDock`: [TopBar](Mangrove/Shell/TopBar.md) · `leftDock`: [Toolbar](Mangrove/Shell/Toolbar.md) · `center`: [Workspace](Mangrove/Shell/Workspace.md) · `rightDock`: [AlertsPanel](Mangrove/Panels/AlertsPanel.md) · `bottomDock`: [StatusBar](Mangrove/Shell/StatusBar.md) |
| `KeyboardShortcuts` | the shortcuts of every action ([KeyboardShortcuts](Mangrove/Shell/KeyboardShortcuts.md)) |

The `objectName`s (`topBar`, `toolbar`, `workspace`, `alertsPanel`, `statusBar`) are used by the UI tests.

## Extending it

Moving or removing a bar takes one line: a dock left empty takes no space
([DockLayout](Mangrove/Controls/DockLayout.md)).

## Links

Loaded by [main](../src/main.md).

---
[↑ Contents](../README.md)
