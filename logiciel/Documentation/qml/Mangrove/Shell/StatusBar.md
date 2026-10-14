# StatusBar

> QML · module `Mangrove.Shell` — [`StatusBar.qml`](../../../../qml/Mangrove/Shell/StatusBar.qml)

Bottom bar: the name of the active tool on the left, the application version on the right.

## Contents

| Side | Item |
|---|---|
| `leading` | name of `EditorActions.activeTool` (`objectName: "activeToolLabel"`) |
| `trailing` | `Qt.application.version` (set by [main](../../../src/main.md)), hidden when empty |

Height: `Theme.statusBarHeight`.

## Links

Inherits from [Bar](../Controls/Bar.md).

---
[↑ Contents](../../../README.md)
