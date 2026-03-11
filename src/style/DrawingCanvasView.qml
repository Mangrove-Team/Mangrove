import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import Mangrove
Item {
    id: root
    property alias canvasItem: canvas

    signal savePlanRequested()

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

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

            Button {
                id: saveButton
                text: "Save Plan"
                Layout.minimumWidth: 120
                background: Rectangle {
                    color: "#2ecc71"
                    border.width: 2
                    radius: 4
                }
                onClicked: savePlanRequested()
            }

            Item { Layout.fillWidth: true }
        }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "white"
            border.color: "black"
            border.width: 2
            radius: 4

            DrawingCanvasBackend {
                id: canvas
                anchors.fill: parent
                anchors.margins: 0
                penColor: "black"
                penWidth: 2
                brushMode: DrawingCanvasBackend.Freehand

                onRectangleDrawn: function(width, height) {
                    rectangleDimensionMenu.rectangleWidth = width
                    rectangleDimensionMenu.rectangleHeight = height
                    rectangleDimensionMenu.duplicateRect = 1
                    rectangleDimensionMenu.spacing = 20

                    widthInput.text = width.toFixed(1)
                    heightInput.text = height.toFixed(1)
                    spaceInput.text = "20.0"
                    duplicateInput.text = "1"
                    rectangleDimensionMenu.visible = true
                }
            }
        }

        ColorDialog {
            id: colorDialog
            title: "Choose Pen Color"
            selectedColor: canvas.penColor
            onAccepted: canvas.penColor = selectedColor
        }
    }

    Rectangle {
        id: rectangleDimensionMenu
        visible: false
        width: 240
        height: contentLayout.implicitHeight + 20
        color: "#e0e0e0"
        border.color: "#555"
        border.width: 2
        radius: 6
        z: 1000

        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 20
        anchors.bottomMargin: 20

        property real rectangleWidth: 0
        property real rectangleHeight: 0
        property real spacing: 20
        property int duplicateRect: 1

        function getSafeWidth() {
            var w = parseFloat(widthInput.text)
            return (isNaN(w) || w <= 0) ? rectangleWidth : w
        }

        function getSafeHeight() {
            var h = parseFloat(heightInput.text)
            return (isNaN(h) || h <= 0) ? rectangleHeight : h
        }

        function getSafeDuplicate() {
            var d = parseInt(duplicateInput.text)
            return (isNaN(d) || d < 1) ? 1 : d
        }

        function getSafeSpacing() {
            var s = parseFloat(spaceInput.text)
            return (isNaN(s) || s < 0) ? spacing : s
        }

        ColumnLayout {
            id: contentLayout
            anchors.fill: parent
            anchors.margins: 10
            spacing: 8

            Label {
                text: "Rectangle Dimensions"
                font.bold: true
                Layout.fillWidth: true
            }

            RowLayout {
                spacing: 8
                Label {
                    text: "Width:"
                    Layout.minimumWidth: 50
                }
                TextField {
                    id: widthInput
                    Layout.fillWidth: true
                    text: rectangleDimensionMenu.rectangleWidth.toFixed(1)
                    selectByMouse: true
                    validator: DoubleValidator {
                        bottom: 0.1; decimals: 2
                    }
                    onAccepted: {
                        canvas.updateLastRectangleDimensions(
                            rectangleDimensionMenu.getSafeWidth(),
                            rectangleDimensionMenu.getSafeHeight(),
                            rectangleDimensionMenu.getSafeDuplicate(),
                            rectangleDimensionMenu.getSafeSpacing()
                        )
                    }

                    MouseArea {
                        anchors.fill: parent
                        propagateComposedEvents: true
                        acceptedButtons: Qt.NoButton

                        property int accumulatedDelta: 0
                        property int threshold: 120

                        onWheel: function(wheel) {
                            accumulatedDelta += wheel.angleDelta.y

                            if (Math.abs(accumulatedDelta) >= threshold) {
                                let steps = Math.floor(Math.abs(accumulatedDelta) / threshold)
                                let direction = accumulatedDelta > 0 ? -1 : 1

                                let widthModifier = 5
                                if (wheel.modifiers & Qt.ControlModifier) {
                                    widthModifier = 0.1
                                }
                                if (wheel.angleDelta.y < 0) {
                                    let newWidth = rectangleDimensionMenu.getSafeWidth()
                                    if (wheel.modifiers & Qt.ShiftModifier) {
                                        newWidth = Math.round(newWidth + widthModifier)
                                    } else {
                                        newWidth += widthModifier
                                    }
                                    widthInput.text = Math.max(0.1, newWidth).toFixed(1)
                                } else if (wheel.angleDelta.y > 0) {
                                    let newWidth = rectangleDimensionMenu.getSafeWidth()
                                    if (wheel.modifiers & Qt.ShiftModifier) {
                                        newWidth = Math.round(newWidth - widthModifier)
                                    } else {
                                        newWidth -= widthModifier
                                    }
                                    widthInput.text = Math.max(0.1, newWidth).toFixed(1)
                                }
                                canvas.updateLastRectangleDimensions(
                                    rectangleDimensionMenu.getSafeWidth(),
                                    rectangleDimensionMenu.getSafeHeight(),
                                    rectangleDimensionMenu.getSafeDuplicate(),
                                    rectangleDimensionMenu.getSafeSpacing()
                                )
                                accumulatedDelta = 0
                            }
                            wheel.accepted = false
                        }

                    }

                }
            }

            RowLayout {
                spacing: 8
                Label {
                    text: "Height:"
                    Layout.minimumWidth: 50
                }
                TextField {
                    id: heightInput
                    Layout.fillWidth: true
                    text: rectangleDimensionMenu.rectangleHeight.toFixed(1)
                    selectByMouse: true
                    validator: DoubleValidator {
                        bottom: 0.1; decimals: 2
                    }
                    onAccepted: {
                        canvas.updateLastRectangleDimensions(
                            rectangleDimensionMenu.getSafeWidth(),
                            rectangleDimensionMenu.getSafeHeight(),
                            rectangleDimensionMenu.getSafeDuplicate(),
                            rectangleDimensionMenu.getSafeSpacing()
                        )
                    }

                    MouseArea {
                        anchors.fill: parent
                        propagateComposedEvents: true
                        acceptedButtons: Qt.NoButton

                        property int accumulatedDelta: 0
                        property int threshold: 120

                        onWheel: function(wheel) {
                            accumulatedDelta += wheel.angleDelta.y

                            if (Math.abs(accumulatedDelta) >= threshold) {
                                let steps = Math.floor(Math.abs(accumulatedDelta) / threshold)
                                let direction = accumulatedDelta > 0 ? -1 : 1

                                let heightModifier = 5
                                if (wheel.modifiers & Qt.ControlModifier) {
                                    heightModifier = 0.1
                                }
                                if (wheel.angleDelta.y < 0) {
                                    let newHeight = rectangleDimensionMenu.getSafeHeight()
                                    if (wheel.modifiers & Qt.ShiftModifier) {
                                        newHeight = Math.round(newHeight + heightModifier)
                                    } else {
                                        newHeight += heightModifier
                                    }
                                    heightInput.text = Math.max(0.1, newHeight).toFixed(1)
                                } else if (wheel.angleDelta.y > 0) {
                                    let newHeight = rectangleDimensionMenu.getSafeHeight()
                                    if (wheel.modifiers & Qt.ShiftModifier) {
                                        newHeight = Math.round(newHeight - heightModifier)
                                    } else {
                                        newHeight -= heightModifier
                                    }
                                    heightInput.text = Math.max(0.1, newHeight).toFixed(1)
                                }
                                canvas.updateLastRectangleDimensions(
                                    rectangleDimensionMenu.getSafeWidth(),
                                    rectangleDimensionMenu.getSafeHeight(),
                                    rectangleDimensionMenu.getSafeDuplicate(),
                                    rectangleDimensionMenu.getSafeSpacing()
                                )
                                accumulatedDelta = 0
                            }
                            wheel.accepted = false
                        }

                    }
                }
            }

            RowLayout {
                spacing: 8
                Label {
                    text: "Duplicate:"
                    Layout.minimumWidth: 50
                }
                TextField {
                    id: duplicateInput
                    Layout.fillWidth: true
                    text: rectangleDimensionMenu.duplicateRect.toString()
                    selectByMouse: true
                    validator: IntValidator {
                        bottom: 1
                    }
                    onAccepted: {
                        canvas.updateLastRectangleDimensions(
                            rectangleDimensionMenu.getSafeWidth(),
                            rectangleDimensionMenu.getSafeHeight(),
                            rectangleDimensionMenu.getSafeDuplicate(),
                            rectangleDimensionMenu.getSafeSpacing()
                        )
                    }

                    MouseArea {
                        anchors.fill: parent
                        propagateComposedEvents: true
                        acceptedButtons: Qt.NoButton

                        property int accumulatedDelta: 0
                        property int threshold: 120

                        onWheel: function(wheel) {
                            accumulatedDelta += wheel.angleDelta.y

                            if (Math.abs(accumulatedDelta) >= threshold) {
                                let steps = Math.floor(Math.abs(accumulatedDelta) / threshold)
                                let direction = accumulatedDelta > 0 ? -1 : 1
                                let duplicateModifier = 1
                                if (wheel.modifiers & Qt.ControlModifier) {
                                    duplicateModifier = 5
                                }
                                if (wheel.angleDelta.y < 0) {
                                    let newDuplicate = rectangleDimensionMenu.getSafeDuplicate()
                                    newDuplicate += duplicateModifier
                                    duplicateInput.text = Math.max(1, newDuplicate).toString()
                                } else if (wheel.angleDelta.y > 0) {
                                    let newDuplicate = rectangleDimensionMenu.getSafeDuplicate()
                                    newDuplicate -= duplicateModifier
                                    duplicateInput.text = Math.max(1, newDuplicate).toString()
                                }
                                canvas.updateLastRectangleDimensions(
                                    rectangleDimensionMenu.getSafeWidth(),
                                    rectangleDimensionMenu.getSafeHeight(),
                                    rectangleDimensionMenu.getSafeDuplicate(),
                                    rectangleDimensionMenu.getSafeSpacing()
                                )
                                accumulatedDelta = 0
                            }
                            wheel.accepted = false
                        }

                    }
                }
            }

            RowLayout {
                spacing: 8
                enabled: rectangleDimensionMenu.getSafeDuplicate() > 1
                Label {
                    text: "Spacing:"
                    Layout.minimumWidth: 50
                }
                TextField {
                    id: spaceInput
                    Layout.fillWidth: true
                    text: rectangleDimensionMenu.spacing.toFixed(1)
                    selectByMouse: true
                    validator: DoubleValidator {
                        bottom: 0 ; decimals: 2
                    }
                    onAccepted: {
                        canvas.updateLastRectangleDimensions(
                            rectangleDimensionMenu.getSafeWidth(),
                            rectangleDimensionMenu.getSafeHeight(),
                            rectangleDimensionMenu.getSafeDuplicate(),
                            rectangleDimensionMenu.getSafeSpacing()
                        )
                    }

                    MouseArea {
                        anchors.fill: parent
                        propagateComposedEvents: true
                        acceptedButtons: Qt.NoButton

                        property int accumulatedDelta: 0
                        property int threshold: 120

                        onWheel: function(wheel) {
                            accumulatedDelta += wheel.angleDelta.y

                            if (Math.abs(accumulatedDelta) >= threshold) {
                                let steps = Math.floor(Math.abs(accumulatedDelta) / threshold)
                                let direction = accumulatedDelta > 0 ? -1 : 1

                                let spacingModifier = 5
                                if (wheel.modifiers & Qt.ControlModifier) {
                                    spacingModifier = 0.1
                                }

                                let newSpacing = rectangleDimensionMenu.getSafeSpacing()
                                if (wheel.modifiers & Qt.ShiftModifier) {
                                    newSpacing = Math.round(newSpacing + (direction * spacingModifier * steps))
                                } else {
                                    newSpacing += (direction * spacingModifier * steps)
                                }

                                spaceInput.text = Math.max(0, newSpacing).toFixed(1)
                                canvas.updateLastRectangleDimensions(
                                    rectangleDimensionMenu.getSafeWidth(),
                                    rectangleDimensionMenu.getSafeHeight(),
                                    rectangleDimensionMenu.getSafeDuplicate(),
                                    rectangleDimensionMenu.getSafeSpacing()
                                )

                                accumulatedDelta = 0
                            }

                            wheel.accepted = false
                        }
                    }
                }
            }

            RowLayout {
                spacing: 8
                Layout.fillWidth: true
                Button {
                    text: "Apply"
                    Layout.fillWidth: true
                    onClicked: {
                        canvas.updateLastRectangleDimensions(
                            rectangleDimensionMenu.getSafeWidth(),
                            rectangleDimensionMenu.getSafeHeight(),
                            rectangleDimensionMenu.getSafeDuplicate(),
                            rectangleDimensionMenu.getSafeSpacing()
                        )
                    }
                }
                Button {
                    text: "Close"
                    Layout.fillWidth: true
                    onClicked: {
                        rectangleDimensionMenu.visible = false
                        canvas.applyRectangleDimensions(
                            rectangleDimensionMenu.getSafeWidth(),
                            rectangleDimensionMenu.getSafeHeight(),
                            rectangleDimensionMenu.getSafeDuplicate(),
                            rectangleDimensionMenu.getSafeSpacing()
                        )
                    }
                }
            }
        }
    }
}
