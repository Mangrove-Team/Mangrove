# DockLayout

> QML · module `Mangrove.Controls` (generic) — [`DockLayout.qml`](../../../../qml/Mangrove/Controls/DockLayout.qml)

Window layout: up to four items on the edges, around a central item.

## Properties

`topDock`, `leftDock`, `center`, `rightDock`, `bottomDock`: `Item`s, each of which can stay empty.

## Rules

- Top and bottom: full width, at their implicit height.
- Left and right: their implicit width.
- The center takes the rest.
- An empty dock takes no space.

## How it works

Each item is placed in a cell (internal `Dock` component) and fills it. Its thickness comes from
its `implicitWidth` / `implicitHeight`: for a [Surface](Surface.md), that is its `thickness`.

## Links

Used by [Main](../../Main.md).

---
[↑ Contents](../../../README.md)
