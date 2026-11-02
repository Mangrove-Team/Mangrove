# ModelList

> QML · module `Mangrove.Controls` (generic) — [`ModelList.qml`](../../../../qml/Mangrove/Controls/ModelList.qml)

Vertical list of the rows of a model, or a message when the model is empty.

## Properties

| Property | Role |
|---|---|
| `model`, `delegate`, `spacing` | those of the inner `ListView` |
| `emptyText` | message shown when there are no rows |
| `empty` | read-only: true when there are no rows |

## Example

```qml
ModelList {
    model: Editor.alerts
    emptyText: qsTr("No issues on this plan.")
    delegate: Card {
        id: card
        required property string message
        width: ListView.view.width
        text: card.message
    }
}
```

---
[↑ Contents](../../../README.md)
