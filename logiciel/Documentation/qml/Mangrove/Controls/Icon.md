# Icon

> QML · module `Mangrove.Controls` (generic) — [`Icon.qml`](../../../../qml/Mangrove/Controls/Icon.qml)

Line icon drawn from the `Icons` paths: crisp at any size and in any color.

## Properties

| Property | Role |
|---|---|
| `name` | key in [Icons](../Theme/Icons.md) |
| `color` | stroke color (`Theme.textPrimary` by default) |

Default size: `Theme.iconSize`.

## How it works

Drawn with `QtQuick.Shapes`. Only the geometry is scaled (24 grid → actual size): the stroke
always keeps `Theme.iconStrokeWidth`.

---
[↑ Contents](../../../README.md)
