import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Mangrove

Window {
    id: rootWindow
    visible: true
    width: 1920
    height: 1080
    minimumWidth: 1200
    minimumHeight: 700
    title: "Mangrove"
    color: "#ffffff"

    property int currentView: 0
    property var openStands: []
    property int currentDrawingIndex: -1

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
            color: "#2c3e50"
            border.color: "#7079f0"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                Button {
                    text: "Dashboard"
                    checkable: true
                    checked: rootWindow.currentView === 0
                    onClicked: {
                        rootWindow.currentView = 0
                        rootWindow.currentDrawingIndex = -1
                    }
                }

                Repeater {
                    model: openStands
                    delegate: Button {
                        text: modelData.name
                        checkable: true
                        checked: rootWindow.currentDrawingIndex === index
                        onClicked: rootWindow.currentDrawingIndex = index
                    }
                }

                Item { Layout.fillWidth: true }

                Label {
                    text: rootWindow.currentView === 0 ? "Stand management" : "Canvas editor"
                    color: "white"
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: rootWindow.currentDrawingIndex === -1

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

        StackLayout {
            id: drawingStackLayout
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: rootWindow.currentDrawingIndex >= 0
            currentIndex: rootWindow.currentDrawingIndex

            Repeater {
                model: rootWindow.openStands

                DrawingCanvasView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }
    }
}