# AlertsPanel

> QML · module `Mangrove.Panels` — [`AlertsPanel.qml`](../../../../qml/Mangrove/Panels/AlertsPanel.qml)

Side panel of constraint violations: an "Alerts" title with a counter, then one card per alert, or "No issues on this plan." when there are none. It checks nothing: it shows what the constraint algorithm gave to `Editor::setAlerts()`.

## How it works

A [SidePanel](../Controls/SidePanel.md) holding a [ModelList](../Controls/ModelList.md) over
`Editor.alerts`; each alert is a [Card](../Controls/Card.md) on `Theme.warningSubtle`, and the
counter turns `Theme.warning` (`badgeColor`) as soon as there is an alert.

## Links

Data: [AlertModel](../../../src/app/models/alert/AlertModel.md) (`Editor.alerts`), filled through
`Editor::setAlerts()` ([guides/ALERTS.md](../../../guides/ALERTS.md)).

---
[↑ Contents](../../../README.md)
