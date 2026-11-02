pragma Singleton

import QtQml

import Mangrove.App
import Mangrove.Controls

// Every user command of the editor. Bars show them with ActionButton and
// KeyboardShortcuts binds their keys: a new command is declared here only.
QtObject {
    // Editing tools, in toolbar order.
    readonly property list<AppAction> tools: [
        ToolAction {
            key: "select"
            kind: Editor.ToolKind.Select
            iconName: "select"
            text: qsTr("Select")
            shortcuts: ["V"]
        },
        ToolAction {
            key: "stand"
            kind: Editor.ToolKind.Stand
            iconName: "stand"
            text: qsTr("Stand")
            shortcuts: ["R"]
        }
    ]

    readonly property AppAction activeTool: {
        for (const tool of tools) {
            if (tool.active)
                return tool;
        }
        return null;
    }

    readonly property AppAction undo: AppAction {
        key: "undo"
        iconName: "undo"
        text: qsTr("Undo")
        shortcuts: ["Ctrl+Z"]
        enabled: Editor.canUndo
        onTriggered: Editor.undo()
    }

    readonly property AppAction redo: AppAction {
        key: "redo"
        iconName: "redo"
        text: qsTr("Redo")
        shortcuts: ["Ctrl+Shift+Z", "Ctrl+Y"]
        enabled: Editor.canRedo
        onTriggered: Editor.redo()
    }

    readonly property AppAction cancel: AppAction {
        key: "cancel"
        text: qsTr("Cancel")
        shortcuts: ["Escape"]
        onTriggered: Editor.cancel()
    }

    // Every action that is not a tool.
    readonly property list<AppAction> commands: [undo, redo, cancel]
}
