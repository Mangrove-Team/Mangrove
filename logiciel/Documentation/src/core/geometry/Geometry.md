# Geometry

> Core layer · pure C++, no Qt — [`Geometry.h`](../../../../src/core/geometry/Geometry.h)

Geometric primitives of the plan, in meters: `Vec2` (point or vector) and `RectM` (axis-aligned rectangle, y pointing down as on screen).

## API

| Item | What it does |
|---|---|
| `Vec2` | `x`, `y`; operators `+`, `-`, `* k`, `==` |
| `RectM::left/top/right/bottom()` | the four edges |
| `topLeft()`, `size()` | top-left corner and size, as `Vec2` |
| `isEmpty()` | width or height ≤ 0 |
| `isValid()` | finite position and size, size ≥ 0, edges that do not overflow. Required to enter a `Plan` |
| `contains(Vec2)`, `contains(RectM)` | containment, edges **included** |
| `intersects(RectM)` | **strict** overlap: two rectangles touching along an edge do not intersect |
| `translated(delta)`, `movedTo(topLeft)` | moved copies |
| `fromCorners(a, b)` | normalized rectangle between two corners given in any order (mouse drawing) |

## Caveats

- Inclusive on one side, strict on the other, on purpose: a click on the edge of a stand selects
  it, and two stands placed edge to edge do not overlap.
- Everything is `constexpr`/`noexcept`, except `isValid()` which uses `std::isfinite`.

## Links

Used by [Plan](../plan/Plan.md), [Conversions](../../app/Conversions.md) and the tools ([ATool](../../app/tools/ATool.md)).

---
[↑ Contents](../../../README.md)
