# IToolContext

> App layer · QML module `Mangrove.App` — [`IToolContext.hpp`](../../../../src/app/tools/IToolContext.hpp)

Interface of everything a tool can read and do. Implemented (privately) by `Editor`.

## API

| Method | What it does |
|---|---|
| `plan()` | the plan, read-only |
| `snapStep()` | step of the snapping grid, in meters |
| `selectedStand()`, `selectStand(id)` | selection |
| `setPreview(rect)` | rectangle shown during a gesture; `std::nullopt` hides it |
| `addStand(rect)` | adds a stand; returns an invalid id if nothing was added |
| `moveStand(id, topLeft)` | moves a stand |

## How it works

`addStand` and `moveStand` go through commands: everything a tool does is undoable. A test can
implement this interface to test a tool on its own, without an Editor.

## Extending it

A tool needs a new capability (delete, resize…): a method here, then its implementation in
[Editor](../Editor.md) (which pushes the matching command).

---
[↑ Contents](../../../README.md)
