import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Mangrove

Rectangle {
    id: toolbar

    property bool drawModeEnabled: false
    property color penColor: "black"
    property int penWidth: 2
    property int brushMode: DrawingCanvasBackend.Freehand

    signal clearRequested()

    color: "#f0f0f0"
    radius: 8

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        spacing: 10

        Button {
            id: drawModeButton
            text: toolbar.drawModeEnabled ? "Dessiner: ON" : "Dessiner: OFF"
            checkable: true
            checked: toolbar.drawModeEnabled
            Layout.minimumWidth: 110
            background: Rectangle {
                color: drawModeButton.checked ? "#2ecc71" : "#bdc3c7"
                border.color: drawModeButton.checked ? "#27ae60" : "#95a5a6"
                border.width: 2
                radius: 4
            }
            onClicked: toolbar.drawModeEnabled = checked
        }

        Rectangle {
            width: 1
            height: 40
            color: "#ccc"
        }

        Button {
            text: "Effacer"
            Layout.minimumWidth: 80
            enabled: toolbar.drawModeEnabled
            onClicked: toolbar.clearRequested()
        }

        Item { width: 10 }

        Label {
            text: "Taille du pinceau:"
            opacity: toolbar.drawModeEnabled ? 1 : 0.4
        }

        Button {
            text: "-"
            implicitWidth: 30; implicitHeight: 30
            enabled: toolbar.drawModeEnabled && penWidthSlider.value > penWidthSlider.from
            onClicked: penWidthSlider.value = Math.max(penWidthSlider.from, penWidthSlider.value - 2)
        }

        Label {
            text: Math.round(penWidthSlider.value)
            Layout.minimumWidth: 24
            horizontalAlignment: Text.AlignHCenter
            opacity: toolbar.drawModeEnabled ? 1 : 0.4
        }

        Button {
            text: "+"
            implicitWidth: 30; implicitHeight: 30
            enabled: toolbar.drawModeEnabled && penWidthSlider.value < penWidthSlider.to
            onClicked: penWidthSlider.value = Math.min(penWidthSlider.to, penWidthSlider.value + 2)
        }

        Slider {
            id: penWidthSlider
            from: 1
            to: 20
            value: 2
            stepSize: 1
            visible: false
            onValueChanged: toolbar.penWidth = Math.round(value)
        }

        Item { width: 10 }

        Label {
            text: "Pinceau:"
            opacity: toolbar.drawModeEnabled ? 1 : 0.4
        }

        Button {
            id: freehandButton
            icon.source: "../../icons/freehand.svg"
            checkable: true; checked: true
            implicitWidth: 30; implicitHeight: 30
            enabled: toolbar.drawModeEnabled
            onClicked: {
                if (checked) {
                    straightLineButton.checked = false
                    rectangleButton.checked = false
                    toolbar.brushMode = DrawingCanvasBackend.Freehand
                } else {
                    checked = true
                }
            }
        }

        Button {
            id: straightLineButton
            icon.source: "../../icons/slash.svg"
            checkable: true
            implicitWidth: 30
            implicitHeight: 30
            enabled: toolbar.drawModeEnabled
            onClicked: {
                if (checked) {
                    freehandButton.checked = false
                    rectangleButton.checked = false
                    toolbar.brushMode = DrawingCanvasBackend.StraightLine
                } else {
                    checked = true
                }
            }
        }

        Button {
            id: rectangleButton
            icon.source: "../../icons/square.svg"
            checkable: true
            implicitWidth: 30
            implicitHeight: 30
            enabled: toolbar.drawModeEnabled
            onClicked: {
                if (checked) {
                    freehandButton.checked = false
                    straightLineButton.checked = false
                    toolbar.brushMode = DrawingCanvasBackend.Rectangle
                } else { checked = true }
            }
        }

        Item { width: 10 }

        Button {
            id: colorButton
            text: "Couleur"
            Layout.minimumWidth: 80
            enabled: toolbar.drawModeEnabled
            background: Rectangle {
                color: toolbar.penColor
                border.color: "#555"
                border.width: 2
                radius: 4
            }
            contentItem: Text {
                text: colorButton.text
                color: {
                    const l = 0.299 * toolbar.penColor.r
                              + 0.587 * toolbar.penColor.g
                              + 0.114 * toolbar.penColor.b
                    return l > 0.5 ? "black" : "white"
                }
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            onClicked: colorDialog.open()
        }


        Item {
            Layout.fillWidth: true
        }
    }

    ColorDialog {
        id: colorDialog
        title: "Choose Pen Color"
        selectedColor: toolbar.penColor
        onAccepted: toolbar.penColor = selectedColor
    }
}

