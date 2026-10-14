# Editor

> App layer · QML module `Mangrove.App` — [`Editor.hpp`](../../../src/app/Editor.hpp) · [`Editor.cpp`](../../../src/app/Editor.cpp)

The single entry point from QML into the application, exposed as the `Editor` QML singleton. It receives pointer events in pixels, converts them to meters, forwards them to the active tool, and exposes the models, the view and the history state to QML.

## How it works

- Owns the [PlanDocument](PlanDocument.md) (plan + history), the [ViewTransform](ViewTransform.md), the
  [StandModel](models/stand/StandModel.md) and [AlertModel](models/alert/AlertModel.md) models
  and one tool per `ToolKind` value.
- Implements [IToolContext](tools/IToolContext.md) **privately**: tools only see it through this
  interface.
- Each `press` opens a new **gesture** (`_gesture` counter). All the moves of one gesture merge into
  a single undo step (see [MoveStandCommand](commands/move/MoveStandCommand.md)).
- Non-finite positions (NaN, ∞) coming from QML are ignored.
- `addStand` / `moveStand` never push a command that could not apply: otherwise the history would
  contain empty steps.
- After every change to the plan (`onPlanChanged`): the selection is cleared if its stand is gone,
  and `hallChanged` is emitted.
- `undo`, `redo` and switching tools cancel the current gesture first.
- **No constraint is checked here.** The constraint algorithm hands its alerts to `setAlerts()`;
  the editor passes them to `AlertModel` and marks every stand they name as `inConflict` in
  `StandModel` ([docs/ALERTS.md](../../../docs/ALERTS.md)).

## API exposed to QML

| Property | What it gives |
|---|---|
| `stands`, `alerts` | the list models |
| `view` | the meters ↔ pixels transform |
| `hall` | the hall, in meters |
| `activeTool` (`Editor.ToolKind`) | the active tool; an unknown value is ignored |
| `preview`, `hasPreview` | the preview rectangle while drawing |
| `canUndo`, `canRedo` | the history state |

| Method | What it does |
|---|---|
| `press(pos)`, `move(pos)`, `release(pos)` | pointer events, in pixels relative to the canvas |
| `cancel()` | drops the current gesture |
| `undo()`, `redo()` | history |

C++ only (not callable from QML):

| Method | What it does |
|---|---|
| `setAlerts(alerts)` | replaces the alerts shown, from the constraint algorithm; an empty list clears them |

## Extending it

- A tool: a value in `ToolKind` + one `_tools.emplace(...)` line in the constructor
  (+ on the QML side, a `ToolAction` in [EditorActions](../../qml/Mangrove/Shell/EditorActions.md)).
- `kSnapStep` (0.5 m) is the step of the tools' snapping grid.

---
[↑ Contents](../../README.md)
