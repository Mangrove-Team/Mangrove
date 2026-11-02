# Icons

> QML · module `Mangrove.Theme` — [`Icons.qml`](../../../../qml/Mangrove/Theme/Icons.qml)

Singleton, icon catalog: for each name, an SVG path (the `d` attribute of a `<path>`) on a 24 × 24 grid, stroked by `Icon`.

## Available icons

`select`, `stand`, `undo`, `redo`.

## Extending it

Add a `"name": "M… L… Z"` entry: strokes only (no fill), on a 24 × 24 grid. The icon can then be
used anywhere with `iconName: "name"`.

## Links

Drawn by [Icon](../Controls/Icon.md).

---
[↑ Contents](../../../README.md)
