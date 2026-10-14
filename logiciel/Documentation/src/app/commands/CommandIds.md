# CommandIds

> App layer · QML module `Mangrove.App` — [`CommandIds.hpp`](../../../../src/app/commands/CommandIds.hpp)

Values of `QUndoCommand::id()`. Two consecutive commands with the same id can merge (`mergeWith`) into a single undo step.

| Value | Used by |
|---|---|
| `MoveStand` | [MoveStandCommand](move/MoveStandCommand.md) |

## Extending it

Add a value for every new command that must be able to merge.

---
[↑ Contents](../../../README.md)
