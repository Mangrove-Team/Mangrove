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

    property alias drawModeEnabled: toolbar.drawModeEnabled

    Shortcut {
        sequence: "Ctrl+Z"
        enabled: toolbar.drawModeEnabled
        onActivated: canvasView.canvasItem.undoLastAction()
    }

    Shortcut {
        sequence: "Ctrl+Y"
        enabled: toolbar.drawModeEnabled
        onActivated: canvasView.canvasItem.redoLastAction()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        DrawingToolbar {
            id: toolbar
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            onClearRequested: canvasView.canvasItem.clearCanvas()
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 12

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: 900

                Map {
                    id: mapView
                    anchors.fill: parent
                }

                DrawingCanvasView {
                    id: canvasView
                    anchors.fill: parent
                    canvasEnabled: toolbar.drawModeEnabled
                    penColor: toolbar.penColor
                    penWidth: toolbar.penWidth
                    brushMode: toolbar.brushMode
                }
            }

            ListStand {
                Layout.preferredWidth: 320
                Layout.fillHeight: true
            }
        }
    }
}