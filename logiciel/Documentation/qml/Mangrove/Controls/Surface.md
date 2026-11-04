# Surface

> QML · module `Mangrove.Controls` (generic) — [`Surface.qml`](../../../../qml/Mangrove/Controls/Surface.qml)

Base of anything docked on an edge of the window (bars, panels): a panel background, a thickness, and a separator line on the center side.

## Properties

| Property | Role |
|---|---|
| `edge` | docking edge: `Qt.TopEdge`, `Qt.BottomEdge`, `Qt.LeftEdge` or `Qt.RightEdge` |
| `thickness` | height of a surface at the top or bottom, width of a surface on the left or right; becomes its `implicitHeight` / `implicitWidth` |
| `horizontal` | read-only: true at the top and bottom |

Example: `Surface { edge: Qt.LeftEdge }` draws its separator line on its right edge, facing the center.

## Links

Subclasses: [Bar](Bar.md), [SidePanel](SidePanel.md).

---
[↑ Contents](../../../README.md)
