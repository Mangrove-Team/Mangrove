import QtQuick

import Mangrove.Controls
import Mangrove.Theme

// Bar down the left of the window: one button per editing tool.
Bar {
    edge: Qt.LeftEdge
    thickness: Theme.toolbarWidth

    Repeater {
        model: EditorActions.tools

        delegate: ActionButton {
            id: toolButton

            required property AppAction modelData

            objectName: "tool_" + toolButton.modelData.key
            appAction: toolButton.modelData
        }
    }
}
