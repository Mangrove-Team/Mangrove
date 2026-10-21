import QtQuick

import Mangrove.Theme

// Every text of the UI. Pick a style: font, size, weight and color follow the theme.
Text {
    id: root

    enum Style {
        Body,
        Title,
        Caption,
        Mono
    }

    property int textStyle: TextLabel.Body
    property bool muted: false
    property bool strong: root.textStyle === TextLabel.Title

    color: root.muted ? Theme.textMuted : Theme.textPrimary
    elide: Text.ElideRight
    font.family: root.textStyle === TextLabel.Mono ? Theme.fontMono : Theme.fontSans
    font.features: Theme.fontFeatures
    font.pixelSize: root.textStyle === TextLabel.Title ? Theme.fontSizeTitle
                  : root.textStyle === TextLabel.Body ? Theme.fontSizeBody
                  : Theme.fontSizeSmall
    font.weight: root.strong ? Font.DemiBold : Font.Normal
}
