import QtQuick

import Mangrove.App
import Mangrove.Theme

Rectangle {
    color: Theme.bgCanvas
    clip: true

    Rectangle {
        id: corner
        width: Theme.rulerSize
        height: Theme.rulerSize
        color: Theme.bgChrome
    }
    Ruler {
        objectName: "horizontalRuler"
        horizontal: true
        anchors { left: corner.right; right: parent.right; top: parent.top }
        height: Theme.rulerSize
    }
    Ruler {
        objectName: "verticalRuler"
        horizontal: false
        anchors { top: corner.bottom; bottom: parent.bottom; left: parent.left }
        width: Theme.rulerSize
    }

    Item {
        id: planArea
        anchors { left: corner.right; top: corner.bottom; right: parent.right; bottom: parent.bottom }
        Component.onCompleted: Editor.view.fit(Editor.hall, Qt.size(width, height), 48)
    }
}
