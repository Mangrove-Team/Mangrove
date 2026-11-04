import QtQuick

import Mangrove.Theme

// Small counter tag. Stands out as soon as the value is not zero.
Rectangle {
    id: root

    property int value: 0
    property color highlightColor: Theme.textPrimary
    readonly property bool highlighted: root.value !== 0

    implicitWidth: Math.max(implicitHeight, label.implicitWidth + 2 * Theme.spacing8)
    implicitHeight: label.implicitHeight + Theme.spacing4
    radius: Theme.radiusSmall
    color: root.highlighted ? root.highlightColor : Theme.bgSubtle

    TextLabel {
        id: label
        anchors.centerIn: parent
        text: root.value
        textStyle: TextLabel.Caption
        color: root.highlighted ? Theme.textInverse : Theme.textMuted
    }
}
