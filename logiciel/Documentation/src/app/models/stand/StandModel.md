# StandModel

> App layer · QML module `Mangrove.App` — [`StandModel.hpp`](../../../../../src/app/models/stand/StandModel.hpp) · [`StandModel.cpp`](../../../../../src/app/models/stand/StandModel.cpp)

List model (`QAbstractListModel`) of the stands for QML, in meters. Available through `Editor.stands`.

## Roles

| QML role | Content |
|---|---|
| `standId` | stand id |
| `label` | "S01"… |
| `xm`, `ym`, `widthM`, `heightM` | rectangle, in meters |
| `selected` | true for the selected stand |
| `inConflict` | true for a stand named by an alert (set through `Editor::setAlerts()`) |

## How it works

- Does not copy the data: reads the plan of the [PlanDocument](../../PlanDocument.md) directly.
- Follows the document's fine-grained signals (`beginInsertRows` / `endInsertRows`…): QML only
  updates the affected row.
- `setSelectedStand` only notifies the previously and newly selected stands.
- `setConflictingStands` only notifies the stands whose `inConflict` state changes. It checks
  nothing: the ids come from the constraint algorithm through `Editor::setAlerts()`.

---
[↑ Contents](../../../../README.md)
