import QtQuick
import QtQuick.Controls

Item {
    id: mapRoot
    property var selectedStand: null

    Rectangle {
        id: mapVisual
        anchors.fill: parent
        color: "#2c3e50"
        radius: 10
        border.color: "#7079f0"
        border.width: 2
        clip: true

        Repeater {
            model: planData.stands
            delegate: Rectangle {
                visible: modelData.x > 0 || modelData.y > 0

                x: modelData.x
                y: modelData.y
                width: 40; height: 40
                color: mapRoot.selectedStand === modelData ? "#f1c40f" : "#e74c3c"
                radius: 20
                border.color: "white"
                border.width: 2

                Text {
                    anchors.centerIn: parent
                    text: modelData.name[0]
                    color: "white"
                    font.bold: true
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: mapRoot.selectedStand = modelData
                }
            }
        }

        DropArea {
            anchors.fill: parent
            keys: ["stand"]

            onDropped: (drop) => {
                let stand = drop.source.modelData;
                stand.x = drop.x - 20;
                stand.y = drop.y - 20;
            }
        }
    }
}