# SelectTool

> App layer · QML module `Mangrove.App` — [`SelectTool.hpp`](../../../../../src/app/tools/select/SelectTool.hpp) · [`SelectTool.cpp`](../../../../../src/app/tools/select/SelectTool.cpp)

Select tool (V key): selects the stand under the pointer and drags it, snapped to the grid.

## How it works

| Event | Effect |
|---|---|
| `press` | selects the topmost stand under the pointer (or deselects if there is none); remembers the grab point and the original position |
| `move` | moves the stand so that the grab point follows the pointer; the top-left corner is snapped to the grid |
| `release` | last move, end of the drag |
| `cancel` | puts the stand back at its original position |

Since the Editor numbers gestures, the whole drag counts as a single undo step
([MoveStandCommand](../../commands/move/MoveStandCommand.md)).

## Links

Derives from [ATool](../ATool.md).

---
[↑ Contents](../../../../README.md)
