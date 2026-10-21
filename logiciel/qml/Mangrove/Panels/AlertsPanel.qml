import QtQuick

import Mangrove.App
import Mangrove.Controls
import Mangrove.Theme

// Side panel listing the constraint alerts given to Editor.setAlerts(). It checks
// nothing itself: it only shows the alerts, in the warning color.
SidePanel {
    title: qsTr("Alerts")
    count: Editor.alerts.count
    badgeColor: Theme.warning

    ModelList {
        anchors.fill: parent
        model: Editor.alerts
        emptyText: qsTr("No issues on this plan.")

        delegate: Card {
            id: alertCard

            required property string message

            width: ListView.view.width
            color: Theme.warningSubtle
            text: alertCard.message
        }
    }
}
