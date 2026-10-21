# Alerts — plugging in the constraint algorithm

The app does **not** check constraints. The constraint algorithm, written separately, computes the
alerts and hands them to the editor; the app only shows them.

## 1. The entry point

```cpp
std::vector<mangrove::core::Alert> alerts = yourAlgorithm(plan);
editor.setAlerts(alerts);
```

An [`Alert`](../../src/core/constraints/Alert.h) is a rule id and the stands it names:

```cpp
struct Alert {
    std::string ruleId;          // "overlap", "inside-hall", or your own id
    std::vector<StandId> stands; // the stands involved
};
```

- Each call **replaces** the previous alerts; `setAlerts({})` clears them.
- Nothing is checked: an id that matches no stand is ignored for the colors and shown as "?" in
  the message.
- Undo and redo do not touch the alerts: call `setAlerts()` again after every change to the plan.

## 2. Where to call it

The natural place is `Editor::onPlanChanged()` in [`src/app/Editor.cpp`](../../src/app/Editor.cpp). It
runs after every change to the plan (add, move, undo, redo):

```cpp
void Editor::onPlanChanged()
{
    const core::Plan &current = _document.plan();
    if (_selected && !current.find(_selected))
        selectStand({});
    setAlerts(yourAlgorithm(current)); // your algorithm, in src/core/
    emit hallChanged();
}
```

- Keep the algorithm in `src/core/` (pure C++, no Qt): it can then be tested on its own in
  `tests/core/`.
- It runs on **every mouse move** while a stand is dragged: keep it fast, or the drag will lag.

## 3. What changes on screen

| State | Visual |
|---|---|
| `Editor.alerts` (`AlertModel`) | the alerts panel shows one card per alert, on `Theme.warningSubtle`, and its counter turns `Theme.warning` |
| `inConflict` role of `Editor.stands` (`StandModel`) | true for every stand named by an alert. The plan canvas does not exist yet: it will draw these stands in `Theme.warning` |

## 4. Messages

Each alert's text comes from `AlertModel::message()`
([`src/app/models/alert/AlertModel.cpp`](../../src/app/models/alert/AlertModel.cpp)):

| `ruleId` | Message (English source) | Stands used |
|---|---|---|
| `"overlap"` | Stands %1 and %2 overlap. | `stands[0]`, `stands[1]` |
| `"inside-hall"` | Stand %1 is outside the hall. | `stands[0]` |
| anything else | Constraint "%1" is not met. | — |

To add a message: one `if` in `message()`, in English with `tr()`, then its French translation
(see [TRANSLATIONS.md](TRANSLATIONS.md)).

## 5. Tests

| Test | What it checks |
|---|---|
| `app/tst_editor::alertsMarkTheStandsTheyName` | the named stands get `inConflict`, the others do not; clearing works |
| `app/tst_app_robustness::alertsNamingUnknownStandsAreHarmless` | invalid or unknown ids change no stand and show "?" |
| `app/tst_undo_invariants` | random alerts during an editing session keep the models consistent |
| `ui/tst_ui::alertsShowInWarningColor` | the counter of the real window turns `Theme.warning`, then back |

---

[Project README](../../README.md) · [Testing guide](TESTING.md)
