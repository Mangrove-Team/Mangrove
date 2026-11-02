import QtQuick
import QtQuick.Layouts

// Docks up to four items on the edges around a center item. Top and bottom
// items span the whole width and keep their implicit height; left and right
// items keep their implicit width; the center takes what is left. An empty
// dock takes no space.
Item {
    id: root

    property Item topDock
    property Item leftDock
    property Item center
    property Item rightDock
    property Item bottomDock

    component Dock: Item {
        id: dock

        property Item docked

        implicitWidth: dock.docked ? dock.docked.implicitWidth : 0
        implicitHeight: dock.docked ? dock.docked.implicitHeight : 0
        children: dock.docked ? [dock.docked] : []

        Binding {
            when: dock.docked !== null
            target: dock.docked
            property: "width"
            value: dock.width
        }
        Binding {
            when: dock.docked !== null
            target: dock.docked
            property: "height"
            value: dock.height
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Dock {
            docked: root.topDock
            Layout.fillWidth: true
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            Dock {
                docked: root.leftDock
                Layout.fillHeight: true
            }

            Dock {
                docked: root.center
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Dock {
                docked: root.rightDock
                Layout.fillHeight: true
            }
        }

        Dock {
            docked: root.bottomDock
            Layout.fillWidth: true
        }
    }
}
