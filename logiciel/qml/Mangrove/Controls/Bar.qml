import QtQuick
import QtQuick.Layouts

import Mangrove.Theme

// Base of every bar: a Surface that lines up its items along its length,
// `leading` ones at the start (left or top), `trailing` ones at the end.
// Declared children go to `leading`.
Surface {
    id: root

    default property alias leading: leadingGroup.data
    property alias trailing: trailingGroup.data

    property int padding: Theme.spacing8
    property int leftPadding: root.padding
    property int rightPadding: root.padding
    property int topPadding: root.padding
    property int bottomPadding: root.padding
    // Space between two items of the same group.
    property int spacing: Theme.spacing4

    readonly property int flow: root.horizontal ? GridLayout.LeftToRight : GridLayout.TopToBottom

    GridLayout {
        anchors.fill: parent
        anchors.leftMargin: root.leftPadding
        anchors.rightMargin: root.rightPadding
        anchors.topMargin: root.topPadding
        anchors.bottomMargin: root.bottomPadding
        flow: root.flow
        rowSpacing: 0
        columnSpacing: 0

        GridLayout {
            id: leadingGroup
            flow: root.flow
            rowSpacing: root.spacing
            columnSpacing: root.spacing
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        GridLayout {
            id: trailingGroup
            flow: root.flow
            rowSpacing: root.spacing
            columnSpacing: root.spacing
        }
    }
}
