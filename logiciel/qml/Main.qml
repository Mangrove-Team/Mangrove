import QtQuick

import Mangrove.Controls
import Mangrove.Panels
import Mangrove.Shell
import Mangrove.Theme

// Says what goes where. Nothing else belongs here.
Window {
    width: Theme.windowWidth
    height: Theme.windowHeight
    minimumWidth: Theme.windowMinimumWidth
    minimumHeight: Theme.windowMinimumHeight
    visible: true
    title: qsTr("Mangrove")
    color: Theme.bgChrome

    DockLayout {
        anchors.fill: parent

        topDock: TopBar {
            objectName: "topBar"
        }
        leftDock: Toolbar {
            objectName: "toolbar"
        }
        center: Workspace {
            objectName: "workspace"
        }
        rightDock: AlertsPanel {
            objectName: "alertsPanel"
        }
        bottomDock: StatusBar {
            objectName: "statusBar"
        }
    }

    KeyboardShortcuts {}
}
