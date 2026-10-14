import QtQuick

import Mangrove.Theme

// Base of everything docked on an edge of the window (bars, side panels): a
// panel-colored surface `thickness` thick, with a hairline on the side that
// faces the center.
Rectangle {
    id: root

    // Window edge the surface is docked to: Qt.TopEdge, Qt.BottomEdge,
    // Qt.LeftEdge or Qt.RightEdge.
    property int edge: Qt.TopEdge
    // Height of a top or bottom surface, width of a left or right one.
    property int thickness: 0

    readonly property bool horizontal: root.edge === Qt.TopEdge || root.edge === Qt.BottomEdge

    implicitWidth: root.horizontal ? 0 : root.thickness
    implicitHeight: root.horizontal ? root.thickness : 0
    color: Theme.bgPanel

    Rectangle {
        z: 1
        x: root.edge === Qt.LeftEdge ? root.width - width : 0
        y: root.edge === Qt.TopEdge ? root.height - height : 0
        width: root.horizontal ? root.width : Theme.borderWidth
        height: root.horizontal ? Theme.borderWidth : root.height
        color: Theme.borderDefault
    }
}
