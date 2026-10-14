# AddStandCommand

> App layer · QML module `Mangrove.App` — [`AddStandCommand.hpp`](../../../../../src/app/commands/add/AddStandCommand.hpp) · [`AddStandCommand.cpp`](../../../../../src/app/commands/add/AddStandCommand.cpp)

Adds a stand on top of the others. Its text in the history is "Add stand S03".

## How it works

- The id and the label are reserved **at construction** (`PlanDocument::makeStand`): redo brings
  back exactly the same stand.
- `redo()` inserts the stand at the end of the list (so on top), `undo()` removes it.
- `standId()` returns the stand's id. It is invalid if the plan has no id left; the Editor then
  does not push the command.

## Links

Derives from [APlanCommand](../APlanCommand.md); created by [Editor](../../Editor.md) (`addStand`).

---
[↑ Contents](../../../../README.md)
