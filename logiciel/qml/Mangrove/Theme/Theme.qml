pragma Singleton

import QtQuick

// Single source of every color, spacing, radius, stroke and font of the UI.
QtObject {
    // --- Colors -------------------------------------------------------------
    readonly property color bgChrome: "#f4f4f3"
    readonly property color bgPanel: "#ffffff"
    readonly property color bgCanvas: "#ebebe8"
    readonly property color bgSubtle: "#f0f0ee"
    readonly property color bgPressed: "#e4e4e1"
    readonly property color borderDefault: "#e0e0dd"
    readonly property color borderStrong: "#8a8a87"
    readonly property color textPrimary: "#1c1c1c"
    readonly property color textMuted: "#6b6b68"
    readonly property color textInverse: "#ffffff"
    readonly property color accent: "#1c1c1c"
    // Constraint alerts: counter, alert cards and stands in conflict.
    readonly property color warning: "#c2410c"
    readonly property color warningSubtle: "#fdeee4"

    // --- Spacing ------------------------------------------------------------
    readonly property int spacing4: 4
    readonly property int spacing8: 8
    readonly property int spacing12: 12
    readonly property int spacing16: 16

    // --- Radii and strokes --------------------------------------------------
    readonly property int radiusSmall: 6
    readonly property int radiusMedium: 8
    readonly property int borderWidth: 1

    // --- Typography ---------------------------------------------------------
    readonly property string fontSans: sansLoader.name
    readonly property string fontMono: monoLoader.name
    readonly property var fontFeatures: ({ "tnum": 1 })
    readonly property int fontSizeSmall: 11
    readonly property int fontSizeBody: 13
    readonly property int fontSizeTitle: 15

    // --- Icons (paths in Icons.qml) -------------------------------------------
    readonly property int iconSize: 18
    readonly property real iconStrokeWidth: 1.5

    // --- Layout and behavior ------------------------------------------------
    readonly property int windowWidth: 1280
    readonly property int windowHeight: 800
    readonly property int windowMinimumWidth: 800
    readonly property int windowMinimumHeight: 480
    readonly property int topBarHeight: 44
    readonly property int statusBarHeight: 26
    readonly property int toolButtonSize: 36
    readonly property int barButtonSize: 28
    readonly property int toolbarWidth: toolButtonSize + 2 * spacing8
    readonly property int panelWidth: 288
    readonly property real disabledOpacity: 0.35
    readonly property int tooltipDelay: 500
    readonly property int rulerSize: 24

    // --- Bundled fonts (OFL, see fonts/OFL-*.txt) ---------------------------
    readonly property FontLoader sansLoader: FontLoader { source: "fonts/Inter-Regular.ttf" }
    readonly property FontLoader sansSemiBoldLoader: FontLoader { source: "fonts/Inter-SemiBold.ttf" }
    readonly property FontLoader monoLoader: FontLoader { source: "fonts/JetBrainsMono-Regular.ttf" }
    readonly property FontLoader monoSemiBoldLoader: FontLoader { source: "fonts/JetBrainsMono-SemiBold.ttf" }
}
