# Conversions

> App layer · QML module `Mangrove.App` — [`Conversions.hpp`](../../../src/app/Conversions.hpp)

Value conversions between core types and Qt types: `Vec2` ↔ `QPointF`, `RectM` ↔ `QRectF`, `std::string` → `QString`.

## API

| Function | Direction |
|---|---|
| `toQt(Vec2)`, `toQt(RectM)`, `toQt(std::string)` | core → Qt |
| `toCore(QPointF)`, `toCore(QRectF)` | Qt → core |

## Caveats

No change of unit: meters stay meters. Converting meters ↔ pixels is the job of
[ViewTransform](ViewTransform.md).

---
[↑ Contents](../../README.md)
