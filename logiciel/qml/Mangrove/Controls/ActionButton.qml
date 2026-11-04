import QtQuick

// IconToolButton driven by an AppAction: icon, tooltip, state and click all come from it.
IconToolButton {
    id: root

    property AppAction appAction

    iconName: root.appAction ? root.appAction.iconName : ""
    tooltip: root.appAction ? root.appAction.tooltip : ""
    active: root.appAction ? root.appAction.active : false
    enabled: root.appAction ? root.appAction.enabled : false
    onClicked: {
        if (root.appAction)
            root.appAction.trigger();
    }
}
