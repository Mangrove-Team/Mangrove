# Stand

> Core layer · pure C++, no Qt — [`Stand.h`](../../../../src/core/plan/Stand.h)

A stand placed on the plan: an id, a displayed label ("S01") and a rectangle in meters. A plain value struct, comparable with `==`.

## Caveats

A `Stand` does not validate itself: `Plan::canInsert` is what rejects an invalid or already used
id, or an invalid rectangle.

## Links

[Plan](Plan.md), [Ids](Ids.md), [Geometry](../geometry/Geometry.md).

---
[↑ Contents](../../../README.md)
