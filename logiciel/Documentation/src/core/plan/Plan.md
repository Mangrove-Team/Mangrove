# Plan

> Core layer · pure C++, no Qt — [`Plan.h`](../../../../src/core/plan/Plan.h) · [`Plan.cpp`](../../../../src/core/plan/Plan.cpp)

The plan of an event: the hall (60 × 40 m by default) and the stands placed in it. It is the central data model of the application.

## How it works

- Stands are kept in **drawing order**: the last one is on top. `standAt` therefore walks the list
  backwards to find the topmost one.
- `makeStand` reserves a fresh id and a free label **without inserting** the stand. An undo command
  thus keeps exactly the same stand across undo / redo.
- Ids are never reused: the internal counter only goes up, even when a stand coming from elsewhere
  (undo, a future file) is inserted with a larger id. Once the largest possible id has been used,
  `makeStand` returns an invalid id.
- Labels: "S01", "S02"… If the label is already taken (imported stand), a suffix is added:
  "S07-2".

## API

| Method | What it does |
|---|---|
| `hall()`, `setHall()` | the hall |
| `stands()`, `size()` | the stands, in drawing order |
| `find(id)` | pointer to the stand, or `nullptr` |
| `indexOf(id)` | position in the list, or empty |
| `standAt(point)` | topmost stand under the point, or an invalid id |
| `makeStand(rect)` | prepares a stand (id + label) without inserting it |
| `canInsert(stand)` | valid and unused id, valid rectangle. **Single source of truth** before an insertion |
| `insertStand(index, stand)` | inserts (index clamped to the end); fails if `!canInsert` |
| `removeStand(id)` | removes the stand and returns it |
| `setStandRect(id, rect)` | moves / resizes; fails if the id is unknown or the rectangle is invalid |

## Caveats

From the app, never change the `Plan` directly: go through a command
([APlanCommand](../../app/commands/APlanCommand.md)), otherwise neither the history nor the QML models follow.

## Links

Owned by [PlanDocument](../../app/PlanDocument.md); read by the tools through
[IToolContext](../../app/tools/IToolContext.md).

---
[↑ Contents](../../../README.md)
