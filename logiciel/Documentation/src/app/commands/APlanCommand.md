# APlanCommand

> App layer · QML module `Mangrove.App` — [`APlanCommand.hpp`](../../../../src/app/commands/APlanCommand.hpp) · [`APlanCommand.cpp`](../../../../src/app/commands/APlanCommand.cpp)

Abstract base class of **every** command that changes the plan. It is the only class that can reach the mutation methods of `PlanDocument`.

## API (protected, for subclasses)

| Item | What it does |
|---|---|
| `plan()` | the plan, read-only |
| `insertStand`, `removeStand`, `setStandRect` | mutations, forwarded to [PlanDocument](../PlanDocument.md) |
| `redo()`, `undo()` | **pure virtual**: a command always knows how to go both ways |

## Extending it

```cpp
class DeleteStandCommand final : public APlanCommand {
    // redo(): remember the index, then removeStand(id)
    // undo(): insertStand(index, stand)
};
```

If the command must merge: a value in [CommandIds](CommandIds.md), then `id()` and `mergeWith()`.

A complete example (command, `Editor`, QML, tests) is in [docs/APP_BUTTON.md](../../../../docs/APP_BUTTON.md).

## Links

Concrete classes: [AddStandCommand](add/AddStandCommand.md), [MoveStandCommand](move/MoveStandCommand.md).

---
[↑ Contents](../../../README.md)
