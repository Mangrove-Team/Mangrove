import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: root

    property var standData: null

    width: parent.width * 0.7
    height: parent.height * 0.7
    padding: 20
    modal: false
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    enter: Transition {
        NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 150 }
        NumberAnimation { property: "scale"; from: 0.9; to: 1.0; duration: 150 }
    }

    background: Rectangle {
        color: "#a3d4aa"
        radius: 12
        border.color: "#43a44a"
        border.width: 2
        layer.enabled: true
    }

    contentItem: ColumnLayout {
        spacing: 12

        Text {
            text: standData ? standData.name : "Détails"
            color: "black"
            font.pixelSize: 22
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        Rectangle { height: 1; Layout.fillWidth: true; color: "#34495e" }

        GridLayout {
            columns: 2
            rowSpacing: 8
            columnSpacing: 15

            Text { text: "Position :"; color: "black"; font.pixelSize: 13 }
            Text {
                text: standData ? `X: ${Math.round(standData.x)} | Y: ${Math.round(standData.y)}` : "-"
                color: "white"; font.bold: true
            }

            Button {
                id: addConstraintButton
                text: "Ajouter une contrainte"
                enabled: standData !== null
                onClicked: constraintMenu.open()
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 8

            Component { id: doubleValidatorComponent; DoubleValidator {} }
            Component { id: intValidatorComponent;    IntValidator    {} }

            Text {
                text: "Cartes des contraintes"
                color: "black"
                font.bold: true
                visible: standData && standData.constraintCards.length > 0
            }

            Repeater {
                model: standData ? standData.constraintCards : []

                delegate: Rectangle {
                    required property var modelData
                    property var cardData: modelData
                    Layout.fillWidth: true
                    radius: 8
                    border.width: 1
                    border.color: "#43a44a"
                    color: "#dff2e2"
                    implicitHeight: cardContent.implicitHeight + 16

                    ColumnLayout {
                        id: cardContent
                        anchors.fill: parent
                        anchors.margins: 8
                        spacing: 8

                        Text {
                            text: cardData.name
                            color: "black"
                            font.bold: true
                        }

                        Repeater {
                            model: cardData.fields

                            delegate: RowLayout {
                                required property var modelData
                                spacing: 6
                                visible: modelData.type === "number" || modelData.type === "string" || modelData.type === "int"

                                Text {
                                    text: modelData.label
                                    color: "black"
                                }

                                TextField {
                                    text: String(modelData.value)
                                    inputMethodHints: modelData.type === "number" || modelData.type === "int"
                                        ? Qt.ImhFormattedNumbersOnly
                                        : Qt.ImhNone
                                    validator: modelData.type === "number"
                                        ? doubleValidatorComponent.createObject(null, { bottom: modelData.min !== undefined ? Number(modelData.min) : -1e9 })
                                        : modelData.type === "int"
                                            ? intValidatorComponent.createObject(null, { bottom: modelData.min !== undefined ? parseInt(modelData.min) : -2147483647 })
                                            : null
                                    onEditingFinished: {
                                        if (!standData || text.length === 0)
                                            return;
                                        let val;
                                        if (modelData.type === "number")      val = Number(text);
                                        else if (modelData.type === "int")    val = parseInt(text);
                                        else                                  val = text;
                                        standData.setFieldFromQml(cardData.name, modelData.name, val);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        Menu {
            id: constraintMenu
            x: addConstraintButton.x
            y: addConstraintButton.y + addConstraintButton.height

            Instantiator {
                model: standData ? standData.availableConstraints : []

                delegate: MenuItem {
                    required property string modelData
                    text: modelData
                    enabled: standData && standData.constraints.indexOf(modelData) === -1
                    onTriggered: standData.addConstraintFromQml(modelData)
                }

                onObjectAdded: (index, object) => constraintMenu.insertItem(index, object)
                onObjectRemoved: (index, object) => constraintMenu.removeItem(object)
            }
        }

        Item { Layout.fillHeight: true }

        Button {
            text: "Fermer"
            Layout.alignment: Qt.AlignRight
            onClicked: root.close()

            background: Rectangle {
                color: parent.hovered ? "#43a44a" : "#2ecc71"
                border.color: "#43a44a"
                radius: 4
            }
            contentItem: Text { text: parent.text; color: "white"; horizontalAlignment: Text.AlignHCenter }
        }
    }
}