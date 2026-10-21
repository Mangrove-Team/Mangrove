import QtQuick

import Mangrove.Theme

// Vertical list of the rows of a model, or a message when it has none.
Item {
    id: root

    property alias model: list.model
    property alias delegate: list.delegate
    property alias spacing: list.spacing
    property string emptyText
    readonly property bool empty: list.count === 0

    TextLabel {
        anchors.left: parent.left
        anchors.right: parent.right
        visible: root.empty
        text: root.emptyText
        muted: true
        wrapMode: Text.Wrap
        elide: Text.ElideNone
    }

    ListView {
        id: list
        anchors.fill: parent
        visible: !root.empty
        clip: true
        spacing: Theme.spacing8
    }
}
