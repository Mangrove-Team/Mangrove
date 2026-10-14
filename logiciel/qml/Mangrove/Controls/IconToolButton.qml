import QtQuick
import QtQuick.Controls.Basic

import Mangrove.Theme

// Square button showing a single icon. Knows nothing about the editor:
// the caller decides what `active` means and what a click does.
AbstractButton {
    id: root

    property bool active: false
    property string iconName
    property string tooltip
    property int size: Theme.toolButtonSize

    implicitWidth: root.size
    implicitHeight: root.size
    padding: 0
    hoverEnabled: true
    focusPolicy: Qt.NoFocus

    Accessible.name: root.tooltip
    ToolTip.visible: root.hovered && root.tooltip.length > 0
    ToolTip.text: root.tooltip
    ToolTip.delay: Theme.tooltipDelay

    contentItem: Item {
        Icon {
            anchors.centerIn: parent
            name: root.iconName
            color: root.active ? Theme.textInverse : Theme.textPrimary
            opacity: root.enabled ? 1 : Theme.disabledOpacity
        }
    }

    background: Rectangle {
        radius: Theme.radiusSmall
        color: root.active ? Theme.accent
             : !root.enabled ? "transparent"
             : root.down ? Theme.bgPressed
             : root.hovered ? Theme.bgSubtle
             : "transparent"
    }
}
