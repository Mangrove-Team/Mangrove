# Ruler

> QML · module `Mangrove.Shell` — [`Ruler.qml`](../../../../qml/Mangrove/Shell/Ruler.qml)

Graduated bar along one edge of the plan, in meters. It follows the zoom and the pan of
`Editor.view`, so its graduations stay aligned with the plan.

## Properties

| Property | Type | Role |
|---|---|---|
| `horizontal` | `bool` | `true` for the top ruler, `false` for the left one. Default: `true` |
| `minLabelSpacing` | `real` (read-only) | Minimum space, in pixels, between two labels: 48 |
| `steps` | `list` (read-only) | Allowed spacings between two labels, in meters: 0.5, 1, 2, 5, 10, 20, 50, 100 |

## How it works

It is a `Canvas`: `onPaint` draws the ruler with the HTML5 2D API.

1. `majorStep(scale)` picks the smallest step of `steps` that is at least `minLabelSpacing`
   pixels wide on screen. At zoom 1 (16 px/m), that is 5 m.
2. The major step is split into 5 minor steps. Only the ticks visible in the ruler are drawn:
   the first and last ones come from `meters = (pixel - offset) / scale`.
3. Each tick is placed with `pixel = meters × scale + offset`
   ([ViewTransform](../../../src/app/ViewTransform.md)). Major ticks span the whole thickness and
   carry a label; minor ticks are a quarter of it. Ticks start from the edge facing the plan.
4. A hairline (`Theme.borderDefault`) separates the ruler from the plan.

The code reads "length" and "thickness" instead of x and y, so one component draws both
orientations.

It is redrawn when its size changes and on `Editor.view.transformChanged`.

## Caveats

- **Same origin as the plan**: `Editor.view.offset` is expressed in the coordinates of the plan
  area. The top ruler must start at the same x as the plan area, and the left ruler at the same
  y, otherwise every tick is shifted by the difference. [Workspace](Workspace.md) does this with a
  corner square of `Theme.rulerSize`.
- **Integer loop**: ticks are numbered with an integer index (`meters = i × minor`) rather than by
  adding `minor` in a loop, so floating-point errors never accumulate and `i % 5` reliably finds
  the major ticks.
- **Crisp lines**: positions are rounded then shifted by 0.5 px, so that a 1 px line covers
  exactly one pixel instead of two half pixels.
- A `Canvas` never repaints on its own: any new input that changes the drawing must call
  `requestPaint()`.

## Extending it

- Different label density: change `minLabelSpacing` or `steps` (keep the 1-2-5 sequence).
- Different thickness: `Theme.rulerSize`.

## Links

Uses [ViewTransform](../../../src/app/ViewTransform.md) through
[Editor](../../../src/app/Editor.md) and the colors and fonts of [Theme](../Theme/Theme.md);
placed twice in [Workspace](Workspace.md).

---
[↑ Contents](../../../README.md)
