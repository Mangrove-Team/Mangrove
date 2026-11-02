# Ids

> Core layer · pure C++, no Qt — [`Ids.h`](../../../../src/core/plan/Ids.h)

Strong identifier of a stand, `StandId`: a wrapped `uint64_t` so it is never mistaken for an index or another number.

## API

| Item | What it does |
|---|---|
| `StandId{}` | **invalid** id (value 0) |
| `explicit operator bool` | true if the id is valid |
| `==` | comparison |
| `std::hash<StandId>` | lets it be used as a key of `unordered_map` / `unordered_set` |

## Links

Ids are handed out by [Plan](Plan.md) (`makeStand`) and are never reused.

---
[↑ Contents](../../../README.md)
