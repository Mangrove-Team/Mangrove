# SidePanel

> QML · module `Mangrove.Controls` (generic) — [`SidePanel.qml`](../../../../qml/Mangrove/Controls/SidePanel.qml)

Base of side panels: a `Surface` docked on the right by default, `Theme.panelWidth` wide, with a title, a counter and a content area.

## Properties

| Property | Role |
|---|---|
| `title` | panel title |
| `count` | number shown in a [Badge](Badge.md) (`objectName: "countBadge"`) next to the title; negative = badge hidden |
| `badgeColor` | color of the counter when it is not zero (`Theme.textPrimary` by default) |
| `content` | default property: declared children go into the content area (`anchors.fill: parent` to fill it) |

## Links

Subclass: [AlertsPanel](../Panels/AlertsPanel.md).

---
[↑ Contents](../../../README.md)
