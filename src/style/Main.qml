import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    visible: true
    width: 1920
    height: 1080
    minimumWidth: 1920
    maximumWidth: 1920
    minimumHeight: 1080
    maximumHeight: 1080
    title: "Mangrove Dashboard"
    color: "#1a1a1a"

    Rectangle {
        id: statsBox
        x: 50; y: 50
        width: 1500; height: 980
        color: "#2c3e50"
        radius: 10
        border.color: "#7079f0"
        border.width: 2

        DragHandler { target: parent }
    }

    Rectangle {
        id: listBoxContainer
        x: 1560; y: 50
        width: 300; height: 980
        color: "#2c3e50"
        radius: 10
        border.color: "#7079f0"
        border.width: 2

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 15

            Text {
                text: "Liste des Stands"
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
                clip: true
                spacing: 10

                delegate: Rectangle {
                    width: listView.width
                    height: 50
                    color: "#34495e"
                    radius: 8

                    Text {
                        anchors.centerIn: parent
                        text: modelData.name
                        color: "white"
                        font.pixelSize: 16
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: parent.color = "#415a77"
                        onExited: parent.color = "#34495e"
                    }
                }
            }
        }

        DragHandler { target: parent }
    }
}