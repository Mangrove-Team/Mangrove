pragma Singleton

import QtQml

// Single source of every icon: SVG path data on a 24 x 24 grid, stroked by
// Mangrove.Controls' Icon. To add an icon, add its path here.
QtObject {
    readonly property var paths: ({
        "select": "M6 3.5 L6 18.5 L10 14.6 L12.9 20.6 L15.2 19.5 L12.4 13.6 L18 13.6 Z",
        "stand": "M4 6.5 H20 V17.5 H4 Z",
        "undo": "M9 14 L4 9 L9 4 M4 9 H14.5 A5.5 5.5 0 0 1 14.5 20 H11",
        "redo": "M15 14 L20 9 L15 4 M20 9 H9.5 A5.5 5.5 0 0 0 9.5 20 H13"
    })
}
