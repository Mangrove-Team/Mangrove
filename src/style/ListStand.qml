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

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        Text { text: "Liste des Stands"; color: "black"; Layout.alignment: Qt.AlignHCenter }

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
                        proxy.x = mouseX - 20;
                        proxy.y = mouseY - 20;
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