import QtQuick

import Mangrove.Theme

// Small counter pill. Stands out as soon as the value is not zero.
Rectangle {
    id: root

    property int value: 0
    // Background when the value is not zero.
    property color highlightColor: Theme.accent
    readonly property bool highlighted: root.value !== 0

    implicitWidth: Math.max(implicitHeight, label.implicitWidth + 2 * Theme.spacing8)
    implicitHeight: label.implicitHeight + Theme.spacing4
    radius: height / 2
    color: root.highlighted ? root.highlightColor : Theme.bgSubtle

    TextLabel {
        id: label
        anchors.centerIn: parent
        text: root.value
        textStyle: TextLabel.Mono
        color: root.highlighted ? Theme.textInverse : Theme.textMuted
    }
}
