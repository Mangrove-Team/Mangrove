# Alert

> Core layer · pure C++, no Qt — [`Alert.h`](../../../../src/core/constraints/Alert.h)

A constraint violation: the id of the rule that raised it (`ruleId`) and the stands involved.

## Caveats

An `Alert` holds **no displayed text**: the core does not know the user's language. The translated
message is built on the app side by `AlertModel::message`.

## Links

Produced by the constraint algorithm (written separately) and handed to `Editor::setAlerts()`
([guides/ALERTS.md](../../../guides/ALERTS.md)); displayed through
[AlertModel](../../app/models/alert/AlertModel.md).

---
[↑ Contents](../../../README.md)
