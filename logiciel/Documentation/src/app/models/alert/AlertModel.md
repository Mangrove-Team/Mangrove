# AlertModel

> App layer · QML module `Mangrove.App` — [`AlertModel.hpp`](../../../../../src/app/models/alert/AlertModel.hpp) · [`AlertModel.cpp`](../../../../../src/app/models/alert/AlertModel.cpp)

List model of the constraint alerts for QML (`Editor.alerts`). It turns the core's rule id into a translated message.

## API

| Item | What it does |
|---|---|
| `message`, `ruleId` roles | displayed text, rule id |
| `count` | number of alerts (notified property) |
| `setAlerts(alerts, plan)` | replaces the list; does nothing if it has not changed |
| `message(alert, plan)` | the displayed text: one case per known rule (`"overlap"`, `"inside-hall"`), otherwise a generic message |

## Caveats

- Messages use `arg(a, b)` in a single call: a label containing "%1" is displayed as is instead of
  being interpreted.
- A stand that cannot be found is shown as "?".
- The `"overlap"` and `"inside-hall"` messages are ready for the constraint algorithm.
- Call `Editor::setAlerts()` rather than this model's `setAlerts()`: otherwise the stands'
  `inConflict` state does not follow.

## Extending it

New kind of alert: add its message in `message()`.

## Links

Owned by [Editor](../../Editor.md) (`Editor.alerts`), filled through `Editor::setAlerts()` by the
constraint algorithm ([docs/ALERTS.md](../../../../../docs/ALERTS.md)); displayed by
[AlertsPanel](../../../../qml/Mangrove/Panels/AlertsPanel.md).

---
[↑ Contents](../../../../README.md)
