import QtQuick
import QtQuick.Controls

Rectangle {
    id: ui
    property var standData: null
    property string displayMode: "list"
    property var selectedStand: null

    width: displayMode === "list" ? 280 : 150
    height: displayMode === "list" ? 50 : 150
    radius: displayMode === "list" ? 8 : 20

    color: {
        if (!standData) return "#a3d4aa"
        if (displayMode === "map") {
            if (standImage.status === Image.Ready) return "transparent"
            return (selectedStand === standData) ? "#f1c40f" : "#e74c3c"
        }
        return standData.onMap ? "#43a44a" : "#a3d4aa"
    }

    border.color: (displayMode === "map" && standImage.status !== Image.Ready) ? "#e74c3c" : "transparent"
    border.width: displayMode === "map" ? 2 : 0
    opacity: (displayMode === "list" && standData && standData.onMap) ? 0.5 : 1.0

    Image {
        id: standImage
        anchors.fill: parent
        source: (standData && displayMode === "map" && standData.hasImage) ? "file:plans/" + standData.name + ".png" : ""
        fillMode: Image.PreserveAspectFit
        visible: status === Image.Ready && displayMode === "map"
        asynchronous: true
        cache: false
    }

    Text {
        anchors.centerIn: parent
        visible: !(displayMode === "map" && standImage.status === Image.Ready)
        text: {
            if (!standData) return ""
            return displayMode === "list" ? standData.name : standData.name[0].toUpperCase()
        }
        color: "black"
        font.bold: displayMode === "map"
    }
}