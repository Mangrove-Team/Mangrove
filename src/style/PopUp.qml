import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: root

    property var standData: null

    width: 300
    height: 220
    padding: 20
    modal: false
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    enter: Transition {
        NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 150 }
        NumberAnimation { property: "scale"; from: 0.9; to: 1.0; duration: 150 }
    }

    background: Rectangle {
        color: "#2c3e50"
        radius: 12
        border.color: "#7079f0"
        border.width: 2
        layer.enabled: true
    }

    contentItem: ColumnLayout {
        spacing: 12

        Text {
            text: standData ? standData.name : "Détails"
            color: "#7079f0"
            font.pixelSize: 22
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        Rectangle { height: 1; Layout.fillWidth: true; color: "#34495e" }

        GridLayout {
            columns: 2
            rowSpacing: 8
            columnSpacing: 15

            Text { text: "Position :"; color: "#bdc3c7"; font.pixelSize: 13 }
            Text {
                text: standData ? `X: ${Math.round(standData.x)} | Y: ${Math.round(standData.y)}` : "-"
                color: "white"; font.bold: true
            }

            Text { text: "Puissance :"; color: "#bdc3c7"; font.pixelSize: 13 }
            Text {
                text: standData ? "1500 Watts" : "0 W"
                color: "#f1c40f"; font.bold: true
            }

            Text { text: "Contraintes :"; color: "#bdc3c7"; font.pixelSize: 13 }
            Text {
                text: (standData && standData.constraints.length > 0) ? standData.constraints.join(", ") : "Aucune"
                color: "#2ecc71"
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
            }
        }

        Item { Layout.fillHeight: true }

        Button {
            text: "Fermer"
            Layout.alignment: Qt.AlignRight
            onClicked: root.close()

            background: Rectangle {
                color: parent.hovered ? "#34495e" : "transparent"
                border.color: "#7079f0"
                radius: 4
            }
            contentItem: Text { text: parent.text; color: "white"; horizontalAlignment: Text.AlignHCenter }
        }
    }
}