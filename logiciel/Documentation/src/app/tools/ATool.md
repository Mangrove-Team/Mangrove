# ATool

> App layer · QML module `Mangrove.App` — [`ATool.hpp`](../../../../src/app/tools/ATool.hpp) · [`ATool.cpp`](../../../../src/app/tools/ATool.cpp)

Abstract base class of tools. `move`, `release` and `cancel` do nothing by default: a new tool only implements `press()` and whatever else it needs.

## API

| Item | What it does |
|---|---|
| `move`, `release`, `cancel` | do nothing by default |
| `snap(value, step)`, `snap(Vec2, step)` | (protected) rounds to the nearest multiple of `step`; a step ≤ 0 turns snapping off |

## Extending it

```cpp
// Selects the clicked stand, without moving it.
class PickTool final : public ATool {
public:
    void press(IToolContext &context, const PointerEvent &event) override
    {
        context.selectStand(context.plan().standAt(event.position));
    }
};
```

Then a value in `Editor::ToolKind`, its registration in the [Editor](../Editor.md) constructor,
and a `ToolAction` in [EditorActions](../../../qml/Mangrove/Shell/EditorActions.md).

## Links

Implements [ITool](ITool.md); concrete classes: [SelectTool](select/SelectTool.md),
[StandTool](stand/StandTool.md).

---
[↑ Contents](../../../README.md)
