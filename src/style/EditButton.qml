import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Button {
    width: parent.width
    text: "Edit"
    flat: true
    font.pixelSize: 12

    contentItem: Text {
        text: parent.text
        color: "black"
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        color: parent.hovered ? "#43a4ba" : "#2eccb1"
        border.color: "#43a4ba"
        radius: 4
    }

    onClicked: {
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