import QtQuick

import Mangrove.Controls
import Mangrove.Theme

// Bar across the top of the window: document on the left, history on the right.
Bar {
    edge: Qt.TopEdge
    thickness: Theme.topBarHeight
    leftPadding: Theme.spacing16

    DocumentTitle {}

    trailing: [
        ActionButton {
            objectName: "undoButton"
            size: Theme.barButtonSize
            appAction: EditorActions.undo
        },
        ActionButton {
            objectName: "redoButton"
            size: Theme.barButtonSize
            appAction: EditorActions.redo
        }
    ]
}
