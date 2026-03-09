import QtQuick
import QtQuick.Controls

Rectangle {
    id: ui
    property var standData: null
    property string displayMode: "list"

    width: displayMode === "list" ? 280 : 40
    height: displayMode === "list" ? 50 : 40
    radius: displayMode === "list" ? 8 : 20

    color: {
        if (!standData) return "#34495e"
        if (displayMode === "map") return (mainMap.selectedStand === standData) ? "#f1c40f" : "#e74c3c"
        return standData.onMap ? "#2ecc71" : "#34495e"
    }

    border.color: "white"
    border.width: displayMode === "map" ? 2 : 0
    opacity: (displayMode === "list" && standData && standData.onMap) ? 0.5 : 1.0

    Text {
        anchors.centerIn: parent
        text: {
            if (!standData) return ""
            return displayMode === "list" ? standData.name : standData.name[0].toUpperCase()
        }
        color: "white"
        font.bold: displayMode === "map"
    }
}