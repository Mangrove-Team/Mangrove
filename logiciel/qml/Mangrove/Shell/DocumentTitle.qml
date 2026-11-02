import QtQuick
import QtQuick.Layouts

import Mangrove.Controls
import Mangrove.Theme

// "<logo> / <document>", as shown in the top bar.
RowLayout {
    id: root

    property string documentName: qsTr("Untitled plan")

    spacing: Theme.spacing8

    Image {
        objectName: "logo"
        Layout.preferredWidth: Theme.logoSize
        Layout.preferredHeight: Theme.logoSize
        source: Theme.logo
        sourceSize: Qt.size(Theme.logoSize, Theme.logoSize)
        fillMode: Image.PreserveAspectFit
        Accessible.role: Accessible.Graphic
        Accessible.name: "Mangrove"
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
