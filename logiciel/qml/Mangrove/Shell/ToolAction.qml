import QtQml

import Mangrove.App
import Mangrove.Controls

// An AppAction that makes `kind` the active tool of the editor.
// The behavior of the tool lives in C++ (src/app/tools/), not here.
AppAction {
    id: root

    // Value of Editor.ToolKind.
    property int kind

    active: Editor.activeTool === root.kind
    onTriggered: Editor.activeTool = root.kind
}
