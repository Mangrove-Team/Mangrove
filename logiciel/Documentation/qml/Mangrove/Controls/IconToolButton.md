# IconToolButton

> QML · module `Mangrove.Controls` (generic) — [`IconToolButton.qml`](../../../../qml/Mangrove/Controls/IconToolButton.qml)

Square button showing an icon. It knows nothing about the editor: the caller decides what `active` means and what a click does.

## Properties

| Property | Role |
|---|---|
| `iconName` | icon ([Icons](../Theme/Icons.md)) |
| `tooltip` | tooltip (also the button's accessible name) |
| `active` | checked state: `brand` background, white icon |
| `size` | side of the button (`Theme.toolButtonSize` by default) |
| `enabled` | when disabled: icon at 35 % opacity |

Visual states: hover (`bgSubtle`), pressed (`bgPressed`).

## Links

When the button stands for a command, use [ActionButton](ActionButton.md) instead.

---
[↑ Contents](../../../README.md)
