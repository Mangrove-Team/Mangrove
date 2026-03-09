import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: rootWindow
    visible: true
    width: 1920
    height: 1080
    minimumWidth: 1920
    maximumWidth: 1920
    minimumHeight: 1080
    maximumHeight: 1080
    title: "Mangrove Dashboard"
    color: "#1a1a1a"

    Map {
        id: mainMap
        x: 50; y: 50
        width: 1500; height: 980
        z: 0
    }

    ListStand {
        id: listBoxContainer
        x: 1560; y: 50
        width: 300; height: 980
        color: "#2c3e50"
        radius: 10
        border.color: "#7079f0"
        border.width: 2
    }
}