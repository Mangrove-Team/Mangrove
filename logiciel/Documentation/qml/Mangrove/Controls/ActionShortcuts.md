# ActionShortcuts

> QML · module `Mangrove.Controls` (generic) — [`ActionShortcuts.qml`](../../../../qml/Mangrove/Controls/ActionShortcuts.qml)

Binds the keyboard shortcuts of a list of actions in the window it lives in. A disabled action leaves its keys free.

## Properties

`actions`: a list of [AppAction](AppAction.md)s.

## How it works

One `Shortcut` per action (created by a `Repeater`): its sequences are `shortcuts`, it follows
`enabled`, and it calls `trigger()`.

## Links

Used by [KeyboardShortcuts](../Shell/KeyboardShortcuts.md).

---
[↑ Contents](../../../README.md)
