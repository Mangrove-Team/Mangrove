import QtQuick

import Mangrove.Theme

// Rounded block holding a wrapped text, sized to fit it.
Rectangle {
    id: root

    property alias text: label.text

    implicitHeight: label.implicitHeight + 2 * Theme.spacing12
    radius: Theme.radiusMedium
    color: Theme.bgSubtle

    TextLabel {
        id: label
        anchors.fill: parent
        anchors.margins: Theme.spacing12
        wrapMode: Text.Wrap
        elide: Text.ElideNone
    }
}
