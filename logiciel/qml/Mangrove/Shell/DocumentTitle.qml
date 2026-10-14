import QtQuick
import QtQuick.Layouts

import Mangrove.Controls
import Mangrove.Theme

// "Mangrove / <document>", as shown in the top bar.
RowLayout {
    id: root

    property string documentName: qsTr("Untitled plan")

    spacing: Theme.spacing8

    TextLabel {
        text: "Mangrove"
        strong: true
    }

    TextLabel {
        text: "/"
        color: Theme.borderStrong
    }

    TextLabel {
        objectName: "documentTitle"
        Layout.fillWidth: true
        text: root.documentName
        muted: true
    }
}
