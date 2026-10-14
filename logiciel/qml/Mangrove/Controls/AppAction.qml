import QtQml

// A user command, independent of how it is shown: ActionButton turns it into a
// button, ActionShortcuts into keyboard shortcuts.
QtObject {
    id: root

    // Stable name, used to build object names ("tool_select").
    property string key
    property string iconName
    property string text
    // Key sequences such as "Ctrl+Z"; the first one is shown in tooltips.
    property list<string> shortcuts
    property bool enabled: true
    // Checked state of a mode, such as the current tool.
    property bool active: false

    readonly property string tooltip: root.shortcuts.length > 0
                                      ? qsTr("%1 (%2)").arg(root.text).arg(root.shortcuts[0])
                                      : root.text

    signal triggered

    function trigger() {
        if (root.enabled)
            root.triggered();
    }
}
