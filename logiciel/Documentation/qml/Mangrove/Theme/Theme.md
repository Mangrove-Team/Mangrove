# Theme

> QML · module `Mangrove.Theme` — [`Theme.qml`](../../../../qml/Mangrove/Theme/Theme.qml)

Singleton, single source of colors, spacings, radii, strokes, fonts, icon sizes and bar dimensions. No hard-coded visual value anywhere else.

## Contents

| Group | Tokens |
|---|---|
| Colors | `bgChrome`, `bgPanel`, `bgCanvas`, `bgSubtle`, `bgPressed`, `borderDefault`, `borderStrong`, `textPrimary`, `textMuted`, `textInverse`, `accent`, `warning` and `warningSubtle` (constraint alerts) |
| Spacings | `spacing4`, `spacing8`, `spacing12`, `spacing16` |
| Radii and strokes | `radiusSmall`, `radiusMedium`, `borderWidth` |
| Typography | `fontSans` (Inter), `fontMono` (JetBrains Mono), `fontFeatures` (tabular figures), `fontSizeSmall` (11), `fontSizeBody` (13), `fontSizeTitle` (15) |
| Icons | `iconSize` (18), `iconStrokeWidth` (1.5) |
| Layout | `windowWidth/Height`, `windowMinimumWidth/Height`, `topBarHeight` (44), `statusBarHeight` (26), `toolButtonSize` (36), `barButtonSize` (28), `toolbarWidth`, `panelWidth` (288), `disabledOpacity`, `tooltipDelay` |
| Fonts | Inter and JetBrains Mono, regular and semi-bold, embedded (OFL license, `fonts/OFL-*.txt`), loaded by `FontLoader` |

## Font rules

- **Inter** (`fontSans`) for the whole interface, with tabular figures (`fontFeatures`) so surfaces,
  powers and stand numbers line up in columns.
- **JetBrains Mono** (`fontMono`) only for technical values: coordinates, measures, stand references.
  A free name typed by the user stays in Inter.
- The condensed display face of the brand (logo, website titles) is never used in the app.

## Usage

`import Mangrove.Theme` then `Theme.spacing8`, `Theme.textMuted`…

## Extending it

Any new visual value becomes a token here rather than a constant inside a component. A dark theme
would only touch this file.

---
[↑ Contents](../../../README.md)
