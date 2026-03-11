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
        rootWindow.drawModeEnabled = true
    }
}