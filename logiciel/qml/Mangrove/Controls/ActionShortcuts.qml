import QtQuick

// Binds the keyboard shortcuts of `actions` in the window it lives in.
// A disabled action leaves its keys free.
Item {
    id: root

    property list<AppAction> actions

    Repeater {
        model: root.actions

        delegate: Item {
            id: entry

            required property AppAction modelData

            Shortcut {
                sequences: Array.from(entry.modelData.shortcuts)
                enabled: entry.modelData.enabled
                onActivated: entry.modelData.trigger()
            }
        }
    }
}
