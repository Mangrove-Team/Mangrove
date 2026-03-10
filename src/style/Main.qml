import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Mangrove

Window {
    id: rootWindow
    visible: true
    width: 1920
    height: 1000
    minimumWidth: 1920
    minimumHeight: 1000
    maximumHeight: 1920
    maximumWidth: 1000
    title: "Mangrove"
    color: "#a3d4aa"

    property int currentView: 0

    Shortcut {
        sequence: "Ctrl+Z"
        enabled: rootWindow.currentView === 1
        onActivated: drawingView.canvasItem.undoLastAction()
    }

    Shortcut {
        sequence: "Ctrl+Y"
        enabled: rootWindow.currentView === 1
        onActivated: drawingView.canvasItem.redoLastAction()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 56
            radius: 10
            color: "#ecf0f1"
            border.color: "#a3d4aa"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                Button {
                    text: "Dashboard"
                    checkable: true
                    checked: rootWindow.currentView === 0
                    onClicked: rootWindow.currentView = 0
                }

                Button {
                    text: "Drawing"
                    checkable: true
                    checked: rootWindow.currentView === 1
                    onClicked: rootWindow.currentView = 1
                }

                Item { Layout.fillWidth: true }

                Label {
                    text: rootWindow.currentView === 0 ? "Stand management" : "Canvas editor"
                    color: "white"
                }
            }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: rootWindow.currentView

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                RowLayout {
                    anchors.fill: parent
                    spacing: 12

                    Map {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.minimumWidth: 900
                        selectedStand: selectedStand
                    }

                    ListStand {
                        Layout.preferredWidth: 320
                        Layout.fillHeight: true
                        selectedStand: selectedStand
                    }
                }

                property var selectedStand: null
            }

            DrawingCanvasView {
                id: drawingView
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}