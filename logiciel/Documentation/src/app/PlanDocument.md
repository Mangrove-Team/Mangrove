# PlanDocument

> App layer · QML module `Mangrove.App` — [`PlanDocument.hpp`](../../../src/app/PlanDocument.hpp) · [`PlanDocument.cpp`](../../../src/app/PlanDocument.cpp)

Owns the plan and its undo history (`QUndoStack`). It guarantees that **every** change to the plan is undoable and announced through signals.

## How it works

- The mutation methods (`insertStand`, `removeStand`, `setStandRect`) are **private**. Only
  [APlanCommand](commands/APlanCommand.md) can reach them (`friend`): the plan cannot be changed
  without going through a command.
- Each change emits fine-grained signals (`aboutToInsertStand` / `standInserted`,
  `aboutToRemoveStand` / `standRemoved`, `standChanged`), then `planChanged`. The models follow the
  former, the [Editor](Editor.md) the latter.
- `insertStand` checks `Plan::canInsert` **before** emitting `aboutToInsertStand`: a model never
  starts an insertion that would fail.

## API

| Method | What it does |
|---|---|
| `plan()` | the plan, read-only |
| `undoStack()` | the history |
| `makeStand(rect)` | reserves an id and a label for a command that will insert the stand later |

## Extending it

New change to the plan: a private method here (with its signals), plus its protected accessor in
[APlanCommand](commands/APlanCommand.md).

---
[↑ Contents](../../README.md)
