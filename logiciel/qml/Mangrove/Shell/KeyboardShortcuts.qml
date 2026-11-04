import QtQuick

import Mangrove.Controls

// Keyboard shortcuts of every editor action, whatever the bars show.
Item {
    ActionShortcuts {
        actions: EditorActions.tools
    }

    ActionShortcuts {
        actions: EditorActions.commands
    }
}
