# TextLabel

> QML · module `Mangrove.Controls` (generic) — [`TextLabel.qml`](../../../../qml/Mangrove/Controls/TextLabel.qml)

Every text of the interface. You pick a style; font, size, weight and color follow the theme. No raw `Text` anywhere else.

## Properties

| Property | Role |
|---|---|
| `textStyle` | `TextLabel.Body` (13 px, default), `TextLabel.Title` (15 px, semi-bold), `TextLabel.Caption` (11 px), `TextLabel.Mono` (11 px, monospace) |
| `muted` | dimmed color |
| `strong` | semi-bold; true by default for `Title` |

By default, a text that is too long is cut with "…" on the right. For a wrapping text:
`wrapMode: Text.Wrap` and `elide: Text.ElideNone`.

## Extending it

New style: a value in the `Style` enum, then its case in `font.family` / `font.pixelSize`.

---
[↑ Contents](../../../README.md)
