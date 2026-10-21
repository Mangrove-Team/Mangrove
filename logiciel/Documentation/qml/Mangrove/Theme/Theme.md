# Theme

> QML · module `Mangrove.Theme` — [`Theme.qml`](../../../../qml/Mangrove/Theme/Theme.qml)

Singleton, single source of colors, spacings, radii, strokes, fonts, icon sizes and bar dimensions. No hard-coded visual value anywhere else.

## Contents

| Group | Tokens |
|---|---|
| Colors | neutrals `bgChrome`, `bgPanel`, `bgCanvas`, `bgSubtle`, `bgPressed`, `borderDefault`, `borderStrong`, `textPrimary`, `textMuted`, `textInverse`; brand `brand`; status `warning`/`warningSubtle`, `conflict`/`conflictSubtle`, `selection` (see below) |
| Spacings | `spacing4`, `spacing8`, `spacing12`, `spacing16` |
| Radii and strokes | `radiusSmall` (2), `radiusMedium` (4), `radiusButton` (6, icon buttons), `borderWidth` |
| Typography | `fontSans` (Inter), `fontMono` (JetBrains Mono), `fontFeatures` (tabular figures), `fontSizeSmall` (11), `fontSizeBody` (13), `fontSizeTitle` (15) |
| Icons | `iconSize` (18), `iconStrokeWidth` (1.5) |
| Logo | `logo` (`images/mangrove-logo.svg`), `logoSize` (24) |
| Layout | `windowWidth/Height`, `windowMinimumWidth/Height`, `topBarHeight` (44), `statusBarHeight` (26), `toolButtonSize` (36), `barButtonSize` (28), `toolbarWidth`, `panelWidth` (288), `disabledOpacity`, `tooltipDelay` |
| Fonts | Inter and JetBrains Mono, regular and semi-bold, embedded (OFL license, `fonts/OFL-*.txt`), loaded by `FontLoader` |

## Color rules

The colors follow the visual identity: a color on screen that is not neutral carries information.

| Family | Tokens | Use |
|---|---|---|
| Neutrals | `bg*`, `border*`, `text*` | everything else: about 70 % light, 20 % dark (text, borders) |
| Brand | `brand` (`#17853C`) | the only brand color, about 10 % of the screen: active mode, and on the plan a valid placement. Never pastel, never in a gradient |
| Status | `warning` (amber) | constraint alerts: alerts panel, stands that break a rule |
| | `conflict` (red) | conflict between incompatible exhibitors |
| | `selection` (blue) | selection on the plan |

- No decorative color: no second brand color, no accent. Red and blue are never used for decoration,
  or the user can no longer tell whether they carry information.
- No gradient, no pastel green.

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
