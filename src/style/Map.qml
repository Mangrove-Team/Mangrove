import QtQuick
import QtQuick.Controls

Rectangle {
    id: mapArea
    property var selectedStand: null

    PopUp {
        id: infoPopup
        standData: mapArea.selectedStand
        parent: Overlay.overlay
        x: (mapArea.width - width) / 2
        y: 50
    }

    color: "#ecf0f1"
    radius: 10
    border.color: "#a3d4aa"
    border.width: 2
    clip: true

    Repeater {
        model: planData.stands
        delegate: Stand {
            id: standDelegate
            standData: modelData
            displayMode: "map"
            selectedStand: mapArea.selectedStand
            visible: standData.onMap
            x: standData.x
            y: standData.y


            Popup {
                id: actionMenu
                y: -90
                x: -30
                width: 120
                height: 70
                padding: 5

                background: Rectangle {
                    color: "#ecf0f1"
                    radius: 5
                    border.color: "#e74c3c"
                    border.width: 1
                }

                Column {
                    id: menuLayout
                    anchors.fill: parent
                    spacing: 5

                    Button {
                        width: parent.width
                        text: "Supprimer"
                        flat: true
                        font.pixelSize: 12

                        contentItem: Text {
                            text: parent.text
                            color: "#e74c3c"
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: {
                            standData.onMap = false;
                            standData.x = 0;
                            standData.y = 0;
                            actionMenu.close();
                        }
                    }

                    Button {
                        width: parent.width
                        text: "Edit"
                        flat: true
                        font.pixelSize: 12

                        contentItem: Text {
                            text: parent.text
                            color: "#3498db"
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: {
                            actionMenu.close();

                            let alreadyOpen = false
                            for (let i = 0; i < rootWindow.openStands.length; i++) {
                                if (rootWindow.openStands[i] === standData) {
                                    alreadyOpen = true
                                    rootWindow.currentDrawingIndex = i
                                    break
                                }
                            }

                            if (!alreadyOpen) {
                                let newOpenStands = rootWindow.openStands.slice()
                                newOpenStands.push(standData)
                                rootWindow.openStands = newOpenStands
                                rootWindow.currentDrawingIndex = newOpenStands.length - 1
                            }

                            rootWindow.currentView = 1
                        }
                    }
                }
            }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton

                onClicked: (mouse) => {
                    if (mouse.button === Qt.RightButton) {
                        actionMenu.open();
                    } else if (mouse.button === Qt.LeftButton) {
                        infoPopup.standData = standData;
                        infoPopup.open();
                    }
                }
            }
        }
    }

    DropArea {
        anchors.fill: parent
        keys: ["stand"]
        onDropped: (drop) => {
            let data = drop.source.itemData;
            data.x = drop.x - 20;
            data.y = drop.y - 20;
            data.onMap = true;
            mapArea.selectedStand = data;
        }
    }
}