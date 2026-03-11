import QtQuick
import QtQuick.Controls

Rectangle {
    id: ui
    property var standData: null
    property string displayMode: "list"
    property var selectedStand: null

    property real imageVersion: Date.now()

    width: {
        if (displayMode === "list") return 280;
        if (displayMode === "map" && standImage.status === Image.Ready) return standImage.implicitWidth;
        return 40;
    }

    height: {
        if (displayMode === "list") return 50;
        if (displayMode === "map" && standImage.status === Image.Ready) return standImage.implicitHeight;
        return 40;
    }

    radius: displayMode === "list" ? 8 : 20

    color: {
        if (!standData) return "#a3d4aa"
        if (displayMode === "map") {
            if (standImage.status === Image.Ready) return "transparent"
            return (selectedStand === standData) ? "#f1c40f" : "#a3d4aa"
        }
        return standData.onMap ? "#43a44a" : "#a3d4aa"
    }

    border.color: (displayMode === "map" && standImage.status !== Image.Ready) ? "#e74c3c" : "transparent"
    border.width: displayMode === "map" ? 2 : 0
    opacity: (displayMode === "list" && standData && standData.onMap) ? 0.5 : 1.0

    Connections {
        target: standData
        ignoreUnknownSignals: true
        function onHasImageChanged() {
            ui.imageVersion = Date.now()
        }
    }

    Image {
        id: standImage
        anchors.fill: parent
        source: (standData && displayMode === "map" && standData.hasImage) ? "file:plans/" + standData.name + ".svg?v=" + ui.imageVersion : ""
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
            return displayMode === "list" ? standData.name : standData.name[standData.name.length - 1]
        }
        color: "black"
        font.bold: displayMode === "map"
    }
}