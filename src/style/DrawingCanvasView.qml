import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Mangrove

ColumnLayout {
    id: root
    spacing: 8
    property alias canvasItem: canvas

    Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: 60
        color: "#f0f0f0"
        radius: 8

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            spacing: 10

            Button {
                text: "Clear Canvas"
                Layout.minimumWidth: 120
                onClicked: canvas.clearCanvas()
            }

            Item { width: 20 }

            Label { text: "Pen Width:" }

            Button {
                text: "-"
                implicitWidth: 30
                implicitHeight: 30
                enabled: penWidthSlider.value > penWidthSlider.from
                onClicked: penWidthSlider.value = Math.max(penWidthSlider.from, penWidthSlider.value - 2)
            }

            Label {
                text: Math.round(penWidthSlider.value)
                Layout.minimumWidth: 30
                horizontalAlignment: Text.AlignHCenter
            }

            Button {
                text: "+"
                implicitWidth: 30
                implicitHeight: 30
                enabled: penWidthSlider.value < penWidthSlider.to
                onClicked: penWidthSlider.value = Math.min(penWidthSlider.to, penWidthSlider.value + 2)
            }

            Slider {
                id: penWidthSlider
                from: 1
                to: 20
                value: 2
                stepSize: 1
                visible: false
                onValueChanged: canvas.penWidth = Math.round(value)
            }

            Item { width: 20 }

            Label { text: "Brush:" }

            Button {
                id: freehandButton
                icon.source: "../../icons/freehand.svg"
                checkable: true
                checked: true
                implicitWidth: 30
                implicitHeight: 30
                onClicked: {
                    if (checked) {
                        straightLineButton.checked = false
                        rectangleButton.checked = false
                        canvas.brushMode = DrawingCanvasBackend.Freehand
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
                onClicked: {
                    if (checked) {
                        freehandButton.checked = false
                        rectangleButton.checked = false
                        canvas.brushMode = DrawingCanvasBackend.StraightLine
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
                onClicked: {
                    if (checked) {
                        freehandButton.checked = false
                        straightLineButton.checked = false
                        canvas.brushMode = DrawingCanvasBackend.Rectangle
                    } else {
                        checked = true
                    }
                }
            }

            Item { width: 20 }

            Button {
                id: colorButton
                text: "Choose Color"
                Layout.minimumWidth: 120
                background: Rectangle {
                    color: canvas.penColor
                    border.color: "#555"
                    border.width: 2
                    radius: 4
                }
                contentItem: Text {
                    text: colorButton.text
                    color: {
                        const r = canvas.penColor.r
                        const g = canvas.penColor.g
                        const b = canvas.penColor.b
                        const lightness = 0.299 * r + 0.587 * g + 0.114 * b
                        return lightness > 0.5 ? "black" : "white"
                    }
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: colorDialog.open()
            }

            Item { Layout.fillWidth: true }
        }
    }

    DrawingCanvasBackend {
        id: canvas
        Layout.fillWidth: true
        Layout.fillHeight: true
        penColor: "black"
        penWidth: 2
        brushMode: DrawingCanvasBackend.Freehand
    }

    ColorDialog {
        id: colorDialog
        title: "Choose Pen Color"
        selectedColor: canvas.penColor
        onAccepted: canvas.penColor = selectedColor
    }
}
