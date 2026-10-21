pragma Singleton

import QtQuick

// Single source of every color, spacing, radius, stroke and font of the UI.
QtObject {
    // --- Colors -------------------------------------------------------------
    // Three families that never mix: one brand color, status colors that
    // always carry information, and neutrals for everything else. A color that
    // is not neutral on screen must mean something.

    // Neutrals: light grays, white and ink.
    readonly property color bgChrome: "#f7f8f8"
    readonly property color bgPanel: "#ffffff"
    readonly property color bgCanvas: "#eceeee"
    readonly property color bgSubtle: "#f0f2f2"
    readonly property color bgPressed: "#e3e6e6"
    readonly property color borderDefault: "#dce0e0"
    readonly property color borderStrong: "#8a9191"
    readonly property color textPrimary: "#161b1b"
    readonly property color textMuted: "#5c6464"
    readonly property color textInverse: "#ffffff"
    readonly property color brand: "#17853c"
    readonly property color warning: "#b45309"
    readonly property color warningSubtle: "#fcf1e2"
    readonly property color conflict: "#c42b1c"
    readonly property color conflictSubtle: "#fbeae8"
    readonly property color selection: "#1d6fd8"

    // --- Spacing ------------------------------------------------------------
    readonly property int spacing4: 4
    readonly property int spacing8: 8
    readonly property int spacing12: 12
    readonly property int spacing16: 16

    // --- Radii and strokes --------------------------------------------------
    readonly property int radiusSmall: 2
    readonly property int radiusMedium: 4
    readonly property int radiusButton: 6
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

    // --- Logo ---------------------------------------------------------------
    readonly property url logo: Qt.resolvedUrl("images/mangrove-logo.svg")
    readonly property int logoSize: 24

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
