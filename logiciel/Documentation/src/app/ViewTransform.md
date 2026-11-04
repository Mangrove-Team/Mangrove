# ViewTransform

> App layer · QML module `Mangrove.App` — [`ViewTransform.hpp`](../../../src/app/ViewTransform.hpp) · [`ViewTransform.cpp`](../../../src/app/ViewTransform.cpp)

The only place where meters are converted to screen pixels and back: `screen = meters × scale + offset`, with `scale = pixelsPerMeter × zoom`. Also handles zooming and panning the view.

## API (through `Editor.view`)

| Item | What it does |
|---|---|
| `pixelsPerMeter` | 16 by default |
| `zoom` | clamped between 0.1 and 20 |
| `scale` | effective pixels per meter (read-only) |
| `offset` | view offset, in pixels |
| `toMeters`, `toScreen`, `toScreenRect` | conversions |
| `zoomAt(screenPos, factor)` | zooms while keeping the point under the cursor still |
| `wheelZoomAt(screenPos, angleDelta)` | same, from the mouse wheel: × 1.2 per notch (120 units) |
| `pan(delta)` | moves the view |
| `fit(meters, viewport, margin)` | frames an area (the hall) in the center of the view |

## Caveats

- Its inputs come from QML: any value that would make the view non-finite (NaN, ∞, factor ≤ 0…)
  is **ignored**. `transformChanged` is only emitted when something actually changes.
- Not displayed yet: there is no canvas on this branch. It is already tested (`tests/app/`).

---
[↑ Contents](../../README.md)
