# StandTool

> App layer · QML module `Mangrove.App` — [`StandTool.hpp`](../../../../../src/app/tools/stand/StandTool.hpp) · [`StandTool.cpp`](../../../../../src/app/tools/stand/StandTool.cpp)

Stand tool (R key): draws a new stand by dragging a rectangle snapped to the grid.

## How it works

| Event | Effect |
|---|---|
| `press` | sets the anchor, snapped to the grid |
| `move` | shows the preview of the rectangle between the anchor and the pointer |
| `release` | creates the stand and selects it. A plain click (empty rectangle) creates a 4 × 3 m stand (`kDefaultSize`) from the anchor |
| `cancel` | clears the preview; nothing is created |

## Links

Derives from [ATool](../ATool.md); creates the stand through `IToolContext::addStand`
([AddStandCommand](../../commands/add/AddStandCommand.md)).

---
[↑ Contents](../../../../README.md)
