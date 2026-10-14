# AppAction

> QML · module `Mangrove.Controls` (generic) — [`AppAction.qml`](../../../../qml/Mangrove/Controls/AppAction.qml)

A user command, independent of how it is shown: `ActionButton` turns it into a button, `ActionShortcuts` into keyboard shortcuts.

## Properties

| Property | Role |
|---|---|
| `key` | stable name; used for `objectName`s (`tool_select`) |
| `iconName`, `text` | presentation |
| `shortcuts` | key sequences (`"Ctrl+Z"`); the first one is shown in the tooltip |
| `enabled` | when disabled: greyed-out button, inactive shortcuts |
| `active` | checked state of a mode (the current tool, for example) |
| `tooltip` | read-only: "Undo (Ctrl+Z)" |

`triggered` signal; the `trigger()` method only emits it when the action is enabled.

## Example

```qml
AppAction {
    key: "save"
    iconName: "save"            // to be added to Icons.qml
    text: qsTr("Save")
    shortcuts: ["Ctrl+S"]
    onTriggered: Editor.save()  // does not exist yet
}
```

## Links

Specialization: [ToolAction](../Shell/ToolAction.md). Shown by [ActionButton](ActionButton.md)
and [ActionShortcuts](ActionShortcuts.md). Every editor action:
[EditorActions](../Shell/EditorActions.md).

---
[↑ Contents](../../../README.md)
