# PointerEvent

> App layer · QML module `Mangrove.App` — [`PointerEvent.hpp`](../../../../src/app/tools/PointerEvent.hpp)

A pointer event as tools receive it: a position already converted to meters.

## Extending it

A field added here (button, Shift / Ctrl modifiers…) becomes available to every tool without
changing their signatures. It must then be filled in `Editor::pointer`.

## Links

Received by [ITool](ITool.md).

---
[↑ Contents](../../../README.md)
