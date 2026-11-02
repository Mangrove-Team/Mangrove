# ToolAction

> QML · module `Mangrove.Shell` — [`ToolAction.qml`](../../../../qml/Mangrove/Shell/ToolAction.qml)

An `AppAction` that activates an editor tool: it is `active` when `Editor.activeTool` equals its `kind`, and triggering it makes `kind` the active tool.

## Properties

`kind`: a value of `Editor.ToolKind`.

## Caveats

Only the tool's presentation is defined here; its behavior is in C++
([ATool](../../../src/app/tools/ATool.md) and its subclasses).

## Links

Inherits from [AppAction](../Controls/AppAction.md); declared in [EditorActions](EditorActions.md).

---
[↑ Contents](../../../README.md)
