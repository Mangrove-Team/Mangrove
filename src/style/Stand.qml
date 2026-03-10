import QtQuick
import QtQuick.Controls

Item {
    id: ui
    property var standData: null
    property string displayMode: "list"
    property var selectedStand: null

    width: displayMode === "list" ? 280 : 40
    height: displayMode === "list" ? 50 : 40

    Image {
        id: standImage
        anchors.fill: parent
        source: standData ? "file:plans/" + standData.name + ".png" : ""
        fillMode: Image.PreserveAspectFit
        visible: status === Image.Ready && displayMode === "map"
        asynchronous: true
        cache: false
        scale: 3.8
    }

    Rectangle {
        id: fallbackRect
        anchors.fill: parent
        radius: displayMode === "list" ? 8 : 20
        visible: displayMode === "list" || standImage.status !== Image.Ready

    color: {
        if (!standData) return "#a3d4aa"
        if (displayMode === "map") return (selectedStand === standData) ? "#f1c40f" : "#e74c3c"
        return standData.onMap ? "#43a44a": "#a3d4aa"
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
        color: "black"
        font.bold: displayMode === "map"
    }
}