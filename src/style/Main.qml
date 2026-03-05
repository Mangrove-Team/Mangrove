import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    visible: true
    width: 1920
    height: 1080
    title: "Mangrove Dashboard"
    color: "#1a1a1a"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        Text {
            text: "Consommation Totale: " + planData.totalWatt + " W"
            color: "white"
            font.bold: true
            font.pixelSize: 18
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: planData.stands

            delegate: Rectangle {
                width: parent.width
                height: 50
                color: "#2c3e50"
                radius: 5
                border.color: "#7079f0"

                Text {
                    anchors.centerIn: parent
                    text: modelData.name
                    color: "white"
                }
            }
        }
    }
}