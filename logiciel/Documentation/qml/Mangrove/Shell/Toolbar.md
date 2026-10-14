# Toolbar

> QML · module `Mangrove.Shell` — [`Toolbar.qml`](../../../../qml/Mangrove/Shell/Toolbar.qml)

Vertical bar on the left: one button per tool of `EditorActions.tools`, in order.

## How it works

A `Repeater` creates one [ActionButton](../Controls/ActionButton.md) per tool, with
`objectName: "tool_<key>"`. Adding a tool changes nothing here: adding it to
[EditorActions](EditorActions.md) is enough.

## Links

Inherits from [Bar](../Controls/Bar.md).

---
[↑ Contents](../../../README.md)
