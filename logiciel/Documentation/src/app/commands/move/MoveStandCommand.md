# MoveStandCommand

> App layer · QML module `Mangrove.App` — [`MoveStandCommand.hpp`](../../../../../src/app/commands/move/MoveStandCommand.hpp) · [`MoveStandCommand.cpp`](../../../../../src/app/commands/move/MoveStandCommand.cpp)

Moves a stand from a `from` rectangle to a `to` rectangle. A whole drag is only **one** undo step.

## How it works

- Every mouse move creates a command; the `QUndoStack` tries to merge it with the previous one
  (`mergeWith`).
- Merging happens if it is the same stand **and** the same gesture (number given by the Editor on
  each `press`): the original `from` and the latest `to` are kept.
- If after merging `to == from` (back to the starting point), the command becomes obsolete and
  disappears from the history.

## Links

Derives from [APlanCommand](../APlanCommand.md); merge id in [CommandIds](../CommandIds.md);
created by [Editor](../../Editor.md) (`moveStand`).

---
[↑ Contents](../../../../README.md)
