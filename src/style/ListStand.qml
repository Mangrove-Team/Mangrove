import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: sidebar
    color: "#ecf0f1"
    radius: 10
    border.color: "#a3d4aa"
    border.width: 2

    property bool isDraggingAny: false
    property var selectedStand: null

    Item {
        id: header
        width: parent.width
        height: 60
        anchors.top: parent.top
        z: 2

        Text {
            id: titleText
            text: "Liste des Exposants"
            color: "black"
            font.bold: true
            font.pixelSize: 18

            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.verticalCenter: parent.verticalCenter
        }

        Button {
            text: "Ajouter"

            anchors.right: parent.right
            anchors.rightMargin: 15
            anchors.verticalCenter: parent.verticalCenter

            width: 80
            height: 35

            onClicked: {
                const nextIndex = planData.stands.length + 1
                planData.addStandFromQml("Exposant " + nextIndex)
            }
        }
    }

    Rectangle {
        id: separator
        width: parent.width - 40
        height: 1
        color: "#dcdde1"
        anchors.top: header.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        anchors.topMargin: 70

        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: planData.stands
            spacing: 10
            clip: !sidebar.isDraggingAny

            delegate: Item {
                id: delegateRoot
                width: listView.width
                height: 50
                property var itemData: model

                Stand {
                    standData: model
                    displayMode: "list"
                    anchors.fill: parent
                }

                Stand {
                    id: proxy
                    standData: model
                    displayMode: "map"
                    selectedStand: sidebar.selectedStand
                    opacity: 0
                    visible: opacity > 0
                    Drag.active: dragArea.drag.active
                    Drag.source: delegateRoot
                    Drag.keys: ["stand"]
                }

                MouseArea {
                    id: dragArea
                    anchors.fill: parent
                    drag.target: proxy
                    hoverEnabled: !sidebar.isDraggingAny

                    onPressed: {
                        proxy.x = mouseX - 10;
                        proxy.y = mouseY - 10;
                        proxy.opacity = 0.8;
                        sidebar.isDraggingAny = true;
                    }
                    onReleased: {
                        proxy.Drag.drop();
                        proxy.opacity = 0;
                        proxy.x = 0; proxy.y = 0;
                        sidebar.isDraggingAny = false;
                    }
                }
            }
        }
    }
}