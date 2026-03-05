/*
// Mangrove Project
// main.qml
// Description: Main QML application window
// Copyright (c) Mangrove. All rights reserved.
*/

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Mangrove

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 1000
    height: 700
    title: "Mangrove Drawing Canvas"

    Shortcut {
        sequence: "Ctrl+Z"
        onActivated: canvas.undoLastAction()
    }

    Shortcut {
        sequence: "Ctrl+Y"
        onActivated: canvas.redoLastAction()
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            color: "#f0f0f0"

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

                Label {
                    text: "Pen Width:"
                }

                Button {
                    text: "-"
                    implicitWidth: 30
                    implicitHeight: 30
                    enabled: penWidthSlider.value > penWidthSlider.from
                    onClicked: {
                        penWidthSlider.value = Math.max(penWidthSlider.from, penWidthSlider.value - 2)
                    }
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
                    onClicked: {
                        penWidthSlider.value = Math.min(penWidthSlider.to, penWidthSlider.value + 2)
                    }
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

                Label {
                    text: "Brush:"
                }

                Button {
                    id: freehandButton
                    text: "Freehand"
                    checkable: true
                    checked: true
                    Layout.minimumWidth: 90
                    onClicked: {
                        if (checked) {
                            straightLineButton.checked = false
                            canvas.brushMode = DrawingCanvasBackend.Freehand
                        } else {
                            checked = true
                        }
                    }
                }

                Button {
                    id: straightLineButton
                    text: "Straight Line"
                    checkable: true
                    checked: false
                    Layout.minimumWidth: 90
                    onClicked: {
                        if (checked) {
                            freehandButton.checked = false
                            canvas.brushMode = DrawingCanvasBackend.StraightLine
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
                            const r = canvas.penColor.r;
                            const g = canvas.penColor.g;
                            const b = canvas.penColor.b;
                            const lightness = 0.299 * r + 0.587 * g + 0.114 * b;
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
    }

    ColorDialog {
        id: colorDialog
        title: "Choose Pen Color"
        selectedColor: canvas.penColor
        onAccepted: {
            canvas.penColor = selectedColor
        }
    }
}

