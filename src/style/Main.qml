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

    Rectangle {
        id: listBoxContainer
        x: 1560; y: 50
        width: 300; height: 980
        color: "#2c3e50"
        radius: 10
        border.color: "#7079f0"
        border.width: 2
        z: dragActiveGlobal ? 100 : 1

        property bool dragActiveGlobal: false

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 15

            Text {
                text: "Liste des Stands Disponibles "
                color: "white"
                font.pixelSize: 18
                font.bold: true
                Layout.alignment: Qt.AlignHCenter
            }

            ListView {
                id: listView
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: planData.stands
                spacing: 10
                clip: !listBoxContainer.dragActiveGlobal

                delegate: Rectangle {
                    id: standDelegate
                    property var modelData: model

                    width: listView.width
                    height: 50
                    color: "#34495e"
                    radius: 8

                    z: dragMouseArea.drag.active ? 1000 : 1

                    Text {
                        anchors.centerIn: parent
                        text: model.name
                        color: "white"
                        font.pixelSize: 16
                    }

                    MouseArea {
                        id: dragMouseArea
                        anchors.fill: parent
                        drag.target: iconVisual

                        onPressed: {
                            iconVisual.x = mouseX - iconVisual.width/2
                            iconVisual.y = mouseY - iconVisual.height/2
                            listBoxContainer.dragActiveGlobal = true
                        }

                        onReleased: {
                            iconVisual.Drag.drop()
                            listBoxContainer.dragActiveGlobal = false
                            iconVisual.x = 0
                            iconVisual.y = 0
                        }
                        Drag.active: dragMouseArea.drag.active
                        Drag.source: standDelegate
                        Drag.keys: ["stand"]
                    }

                    Rectangle {
                        id: iconVisual
                        width: standDelegate.width
                        height: standDelegate.height
                        color: "#7079f0"
                        radius: 8
                        opacity: dragMouseArea.drag.active ? 0.8 : 0
                        visible: opacity > 0

                        Drag.active: dragMouseArea.drag.active
                        Drag.source: standDelegate
                        Drag.keys: ["stand"]

                        Text {
                            anchors.centerIn: parent
                            text: model.name
                            color: "white"
                            font.pixelSize: 16
                        }
                    }
                }
            }
        }
    }
}