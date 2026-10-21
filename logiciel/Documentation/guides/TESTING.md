# Tests — Mangrove desktop app

This guide explains how to run the tests, which kind of test to write for which change, and how to
write it. The examples can be copied as they are.

## 1. Running the tests

| I want to…                                 | Command                                                                  |
|--------------------------------------------|--------------------------------------------------------------------------|
| Run everything                             | `cmake -S . -B build && cmake --build build && ctest --test-dir build`   |
| Run one layer                              | `ctest --test-dir build -L core` (or `app`, `ui`)                         |
| Run the robustness / security tests        | `ctest --test-dir build -L security`                                      |
| Run one executable, in detail              | `./build/tests/app/tst_editor -v2`                                        |
| Run a single test function                 | `./build/tests/app/tst_editor dragMovesStandAsOneUndoStep`               |
| Run a single row of a data-driven test     | `./build/tests/core/tst_core_robustness "rectValidity:NaN x"`            |
| Hunt flaky tests                           | `ctest --test-dir build -L ui --repeat until-fail:20`                     |
| Run under ASan + UBSan                     | `cmake --preset linux-asan && cmake --build --preset linux-asan && ctest --preset linux-asan` |
| Fuzz (Clang)                               | `cmake --preset linux-fuzz && cmake --build --preset linux-fuzz --target fuzz_plan` then see §6 |
| Lint the QML                               | `cmake --build build --target all_qmllint`                                |
| Check the French translation               | `ctest --test-dir build -L i18n`                                          |

On a machine with little RAM, build with `-j2`: a Ninja build at full parallelism can be killed by
the system (exit code 137).

Random tests print their seed (`random seed: 20261003`). It is fixed, so a failure reproduces
exactly. To explore other sequences: `MANGROVE_TEST_SEED=42 ctest ...`. If a seed makes a test
fail, **keep it** and turn the case into a deterministic test.

## 2. Layout

```
tests/
  CMakeLists.txt        mangrove_add_test(): creates the executable, the ctest test and its labels
  support/              shared helpers (TestSeed.h)
  core/                 pure core, QTEST_APPLESS_MAIN, no Qt dependency in the code under test
  app/                  Editor, commands, tools, models — without loading any QML
  ui/                   real window (qml/Main.qml) offscreen, real mouse/keyboard events
  i18n/                 every displayed text has its French translation (CMake script, no executable)
  fuzz/                 libFuzzer targets (only with -DMANGROVE_FUZZ=ON)
  sanitizers/lsan.supp  known leaks of system libraries (never our code)
```

Every test automatically gets its folder name as a label (`core`, `app`, `ui`, `i18n`).
Add `LABELS security` to tests that check hostile input or invariants.

| File                            | What it guarantees                                                              |
|---------------------------------|---------------------------------------------------------------------------------|
| `core/tst_geometry`             | `RectM`: containment, strict intersection, normalization                        |
| `core/tst_plan`                 | unique ids and labels, insert/remove, `standAt` returns the topmost one          |
| `core/tst_core_robustness`      | NaN/∞/negative sizes rejected, unknown or exhausted ids, label collisions        |
| `core/tst_plan_invariants`      | 5,000 random operations: `Plan` invariants checked after each one                |
| `app/tst_editor`                | user scenarios: draw, move, undo, zoom, alerts marking stands                    |
| `app/tst_app_robustness`        | anything QML can send: NaN, huge coordinates, nonsensical call order, invalid properties |
| `app/tst_undo_invariants`       | random sessions; undoing everything then redoing everything restores the plan; model contract (`QAbstractItemModelTester`) |
| `ui/tst_ui`                     | the real application: bar layout, tool buttons and shortcuts, undo/redo, alerts panel, French interface; any QML warning fails the test |
| `i18n/translationsAreComplete`  | no text of `translations/mangrove_fr.ts` is left untranslated                    |
| `fuzz/fuzz_plan`                | arbitrary bytes → operations on the `Plan`; stops at the first broken invariant  |

## 3. Which test for which change?

| I changed…                                      | I write at least…                                                                |
|-------------------------------------------------|----------------------------------------------------------------------------------|
| Geometry, the `Plan`                            | a test in `core/` + a case in `tst_core_robustness` if the input can be invalid  |
| An alert message (`AlertModel`)                 | a case in `app/tst_app_robustness::malformedAlertsAreDisplayable` — see [ALERTS.md](ALERTS.md) |
| An undoable command                             | `app/`: do / undo / redo; add it to the actions of `tst_undo_invariants`          |
| A tool                                          | `app/tst_editor` (scenario) + `tst_app_robustness` (interrupted gesture, tool change mid-gesture) |
| A `Q_INVOKABLE` method or a `WRITE` property    | `tst_app_robustness`: non-finite values, out-of-range values, unknown enum        |
| A model                                         | plug it into `QAbstractItemModelTester` in `tst_undo_invariants`                  |
| QML (interaction, shortcut, panel)              | `ui/tst_ui`                                                                      |
| A text shown to the user                        | its French translation in `mangrove_fr.ts` (`update_translations`, then `ctest -L i18n`) — see [TRANSLATIONS.md](TRANSLATIONS.md) |
| Loading external data (file, clipboard, network) | a fuzzing target in `fuzz/` **before** merging — see §7                         |
| A bug fix                                       | a test that fails without the fix, named after the expected behavior             |

## 4. Recipes

### 4.1 Core unit test

```cpp
// tests/core/tst_aisle.cpp
#include "plan/Plan.h"

#include <QtTest>

using namespace mangrove::core;

class TestAisle : public QObject {
    Q_OBJECT

private slots:
    void aisleCannotBeNarrowerThanMinimum()
    {
        Plan plan;
        // ...
        QVERIFY(!plan.insertAisle(/* ... */));
    }
};

QTEST_APPLESS_MAIN(TestAisle) // the core needs no Qt application
#include "tst_aisle.moc"
```

Then in `tests/core/CMakeLists.txt`:

```cmake
mangrove_add_test(tst_aisle LIBS Mangrove::Core)
```

### 4.2 Data-driven test (one row = one case)

Prefer it whenever the same behavior is checked on several values. Each row has a readable name in
the report and can be run on its own.

```cpp
void rectValidity_data()
{
    QTest::addColumn<RectM>("rect");
    QTest::addColumn<bool>("valid");
    QTest::newRow("regular") << RectM{1, 2, 4, 3} << true;
    QTest::newRow("NaN x") << RectM{kNaN, 0, 4, 3} << false;
}

void rectValidity()
{
    QFETCH(RectM, rect);
    QFETCH(bool, valid);
    QCOMPARE(rect.isValid(), valid);
}
```

### 4.3 App layer test (like QML, without QML)

Drive `Editor` with **screen pixels**, exactly as the canvas will, and read the result from the
models. Default view: offset (0, 0), 16 px per meter.

```cpp
#include "Editor.hpp"
#include <QtTest>

using namespace mangrove::app;

QPointF px(double xm, double ym) { return {xm * 16.0, ym * 16.0}; }

void TestX::clickPlacesStand()
{
    Editor editor; // a fresh Editor per test: no shared state
    editor.setActiveTool(Editor::ToolKind::Stand);
    editor.press(px(10, 10));
    editor.release(px(10, 10));
    QCOMPARE(editor.stands()->rowCount(), 1);
}

QTEST_GUILESS_MAIN(TestX)
```

`mangrove_add_test(tst_x LIBS Mangrove::App)`.

### 4.4 End-to-end UI test

Start from `tests/ui/tst_ui.cpp`. The key points:

- `init()` creates a **new QML engine per test**, hence a fresh `Editor`.
- A QML warning fails the test (`QQmlEngine::warnings` signal).
- Wait for the window to be **active** before sending shortcuts (`qWaitForWindowActive`), and for
  the layout to be done before computing coordinates (`QTRY_VERIFY`).
- Items are found by `objectName` in the visual tree (`item("topBar")`, `item("tool_stand")`,
  `item("undoButton")`…). Add an `objectName` rather than relying on the shape of the tree. The
  delegates of a `Repeater` are not QObject children of the window: `findChild` does not find
  them, `item()` does.
- For a canvas, convert meters to window positions **with the current view**. If the gesture under
  test moves the view (pan, zoom), compute the points **once, before** the gesture.
- Never use `QTest::qWait(500)` to "give it time": use `QTRY_VERIFY` / `QTRY_COMPARE`.

### 4.5 Random invariant test

For a stateful component (plan, history), describe what must **always** be true, then apply
thousands of random operations and check after each one.

```cpp
#include "TestSeed.h"

std::mt19937 rng(mangrove::test::testSeed()); // fixed seed, printed, can be overridden
for (int step = 0; step < 5000; ++step) {
    applyRandomOperation(plan, rng);
    const QString broken = brokenInvariant(plan);
    if (!broken.isEmpty())
        QFAIL(qPrintable(QStringLiteral("step %1: %2").arg(step).arg(broken)));
}
```

Keep the size bounded (see `kMaxStands`): the invariant check is O(n²) (`indexOf` for every stand) and a slow test ends up disabled. Aim
for under a few seconds.

### 4.6 Checking a Qt model

```cpp
QAbstractItemModelTester tester(editor.stands(),
    QAbstractItemModelTester::FailureReportingMode::QtTest);
// ... any action that changes the model ...
```

The tester checks every signal (`beginInsertRows`/`endInsertRows`, `dataChanged`…). It is what
would catch, for example, an insertion announced to the model and then refused by the `Plan`.

## 5. Conventions

- One test function = one behavior, named like a sentence: `overlapIgnoresTouchingStands`, not
  `test3`.
- Files `tst_<subject>.cpp`, one class `Test<Subject>`, registered with `mangrove_add_test`.
- `QCOMPARE` rather than `QVERIFY(a == b)`: the report shows both values.
- `QVERIFY2(cond, message)` when the condition alone does not say what broke.
- No dependency between test functions; no global state.
- No network access, no file outside the build folder.
- The build must stay warning-free, tests included.
- A bug fix comes **with** its test. Mention where it came from in a comment if that helps
  (`// Found by fuzz_plan: ...`).

## 6. Sanitizers and fuzzing

### ASan + UBSan

`-DMANGROVE_SANITIZERS="address;undefined"` (or the `linux-asan` preset) instruments the whole
project: buffer overflow, use-after-free, leak, undefined behavior (signed integer overflow,
invalid conversion…). Any detection **stops** the process, so the test fails. Run the full suite
under sanitizers before merging anything that touches C++.

Leaks from system libraries (fontconfig for instance) are listed in `tests/sanitizers/lsan.supp`,
with their justification. Only add a line after reading the stack and checking that no frame comes
from our code. **Never suppress a leak from `src/`.**

### libFuzzer

```sh
cmake --preset linux-fuzz && cmake --build --preset linux-fuzz --target fuzz_plan
mkdir -p corpus
./build/linux-fuzz/tests/fuzz/fuzz_plan -max_total_time=600 corpus   # 10 minutes
./build/linux-fuzz/tests/fuzz/fuzz_plan crash-<hash>                 # replay a finding
```

`ctest -L fuzz` only does a short run (200,000 inputs, fixed seed). Real fuzzing is run by hand or
as a nightly job, with a corpus kept from one session to the next.

When the fuzzer finds a case: replay it, understand the scenario, write it as a **deterministic
test** in `tests/core/` (readable, fast, run everywhere), fix it, then check that the `crash-*` file
passes.

To write a new target, copy `fuzz_plan.cpp`: read the bytes as a sequence of operations with
`FuzzedDataProvider`, check the invariants after each operation, call `std::abort()` as soon as one
breaks. Register it with `mangrove_add_fuzz_target(fuzz_x LIBS ...)`.

## 7. Security: what we protect and the checklist

The application does not read files or use the network yet. The untrusted inputs today are:

1. **What QML sends to `Editor`**: positions (NaN is possible as soon as a JS computation goes
   wrong), values written to properties (any `int` for an enum), call order.
2. **Data coming from elsewhere** that the core will soon accept (files, clipboard): arbitrary ids,
   arbitrary labels, extreme coordinates.

Resulting rules, already tested:

- The `Plan` rejects any rectangle that is not finite, has a negative size or whose edges overflow
  (`RectM::isValid`). `Plan::canInsert` is the single source of truth, used **before** notifying
  the models.
- Ids are never reused nor reset; a generated label never reuses an existing label.
- `Editor` ignores non-finite pointer events and never pushes a command that cannot apply
  (otherwise the history contains empty steps).
- `ViewTransform` ignores any input that would make the view non-finite.
- Translated messages use `arg(a, b)`: a label containing `%1` is displayed as is.

Checklist before merging a feature:

- [ ] New input exposed to QML? Test with NaN, ±∞, huge values, unknown enum.
- [ ] New data stored in the core? Validation in the core (not only in the UI) and invalid cases in
      `tst_core_robustness`.
- [ ] Reading external data? Fuzzing target + size limits (number of stands, string length) + test
      of the corrupted/truncated file.
- [ ] New text built from user data? No concatenation inside a format string; multi-argument
      `arg()`.
- [ ] New command? Added to the random actions of `tst_undo_invariants`.
- [ ] Full suite green under `linux-asan`.

## 8. Not covered yet

- **Windows / macOS**: sanitizers and fuzzing are set up for Linux. MSVC only supports
  `MANGROVE_SANITIZERS=address`.
- **Continuous integration**: nothing runs these tests automatically. To wire up: `ctest` +
  `all_qmllint` on every PR, `linux-asan` on every PR that touches C++, nightly fuzzing.
- **Texts missing from the `.ts` file**: if nobody runs `update_translations` after adding a text,
  `ctest -L i18n` cannot see it and the text stays in English. To wire up in CI: run
  `update_translations` and fail if `translations/mangrove_fr.ts` changed.
- **Duplicate labels from outside**: two imported stands can carry the same label. This is a
  product decision (reject on import, rename, or report it as an alert) that has not been made
  yet.
