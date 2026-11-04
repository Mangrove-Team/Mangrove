import QtQuick

import Mangrove.Controls
import Mangrove.Theme

// Bar across the bottom of the window: active tool on the left, version on the right.
Bar {
    edge: Qt.BottomEdge
    thickness: Theme.statusBarHeight
    topPadding: 0
    bottomPadding: 0
    leftPadding: Theme.spacing12
    rightPadding: Theme.spacing12

    TextLabel {
        objectName: "activeToolLabel"
        text: EditorActions.activeTool ? EditorActions.activeTool.text : ""
        textStyle: TextLabel.Caption
        muted: true
    }

    trailing: TextLabel {
        visible: Qt.application.version.length > 0
        text: "v" + Qt.application.version
        textStyle: TextLabel.Mono
        muted: true
    }
}
