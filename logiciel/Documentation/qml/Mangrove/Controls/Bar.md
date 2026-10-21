# Bar

> QML · module `Mangrove.Controls` (generic) — [`Bar.qml`](../../../../qml/Mangrove/Controls/Bar.qml)

Base of every bar. It lines up its items along the bar: `leading` at the start (left or top), `trailing` at the end (right or bottom), and a stretchable space in between. It is horizontal or vertical depending on its `edge`.

## Properties

| Property | Role |
|---|---|
| `leading` | default property: declared children go here |
| `trailing` | items at the end of the bar |
| `padding`, `leftPadding`, `rightPadding`, `topPadding`, `bottomPadding` | inner margins (8 px by default) |
| `spacing` | space between two items of the same group (4 px) |

Also inherits `edge` and `thickness` ([Surface](Surface.md)).

## Example

```qml
Bar {
    edge: Qt.TopEdge
    thickness: Theme.topBarHeight

    TextLabel { text: "Title" }              // leading

    trailing: [
        ActionButton { appAction: EditorActions.undo },
        ActionButton { appAction: EditorActions.redo }
    ]
}
```

## Links

Subclasses: [TopBar](../Shell/TopBar.md), [Toolbar](../Shell/Toolbar.md), [StatusBar](../Shell/StatusBar.md).

---
[↑ Contents](../../../README.md)
