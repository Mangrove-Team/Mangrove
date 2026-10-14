import QtQuick
import QtQuick.Layouts

import Mangrove.Theme

// Base of every side panel: a Surface with a title, an optional counter and a
// content area. Declared children go to the content area and fill it.
Surface {
    id: root

    property string title
    // Shown in a badge next to the title; a negative value hides the badge.
    property int count: -1
    // Color of the counter when it is not zero.
    property color badgeColor: Theme.accent
    default property alias content: body.data

    edge: Qt.RightEdge
    thickness: Theme.panelWidth

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacing16
        spacing: Theme.spacing12

        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.spacing8

            TextLabel {
                Layout.fillWidth: true
                text: root.title
                textStyle: TextLabel.Title
            }

            Badge {
                objectName: "countBadge"
                visible: root.count >= 0
                value: root.count
                highlightColor: root.badgeColor
            }
        }

        Item {
            id: body
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
