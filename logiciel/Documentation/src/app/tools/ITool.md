# ITool

> App layer · QML module `Mangrove.App` — [`ITool.hpp`](../../../../src/app/tools/ITool.hpp)

Interface of an editing tool. A tool turns pointer events (in meters) into changes, only through `IToolContext`: it never sees `Editor` nor QML.

## API

| Method | Called when |
|---|---|
| `press(context, event)` | the button is pressed: start of a gesture |
| `move(context, event)` | the pointer moves with the button pressed |
| `release(context, event)` | the button is released: end of the gesture |
| `cancel(context)` | the gesture is dropped (Escape, tool change, undo / redo); the tool reverts what it changed |

## Extending it

Do not implement `ITool` directly: derive from [ATool](ATool.md).

## Links

Receives an [IToolContext](IToolContext.md) and [PointerEvent](PointerEvent.md)s.

---
[↑ Contents](../../../README.md)
