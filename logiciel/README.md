# Mangrove — desktop app

Event floor plan editor (trade fairs, markets). Qt 6 desktop application (QML + C++20).
The code and its docs are in English; the interface is shown to users in French.

## Build and run

Requirements: CMake ≥ 3.25, a C++20 compiler (GCC ≥ 13, Clang ≥ 17, MSVC 2022), Qt ≥ 6.8
(modules Core, Gui, Qml, Quick, QuickControls2, Test, LinguistTools).

```sh
cmake -S . -B build              # system Qt (MANGROVE_USE_VCPKG=OFF by default)
cmake --build build
ctest --test-dir build           # every test (guide: Documentation/guides/TESTING.md)
cmake --build build --target all_qmllint   # lint every QML module
./build/Mangrove
```

Without Qt installed, the `linux-debug`, `linux-release` and `windows-*` presets turn on
`MANGROVE_USE_VCPKG`: vcpkg is cloned into `external/vcpkg` and builds Qt (slow the first time).

```sh
cmake --preset linux-release && cmake --build --preset linux-release
```

> On Linux, the `qmllint` on the `PATH` may be the Qt 5 one. The Qt 6 one lives in
> `$(qtpaths6 --query QT_HOST_BINS)/qmllint`; the `all_qmllint` target uses it automatically.

## Architecture

Three layers, dependencies go **one way only**: `qml → app → core`.

```
src/core/        Pure C++20, no Qt dependency. Static library mangrove_core.
  geometry/        Vec2, RectM (meters)
  plan/            StandId, Stand, Plan
  constraints/     Alert (a constraint violation, computed outside the app and shown by it)
src/app/         QML module "Mangrove.App" (static): bridge between core and QML.
  Editor           QML singleton, the single entry point from QML; implements IToolContext
  PlanDocument     owns the Plan and the QUndoStack
  ViewTransform    the only meters ↔ pixels conversion
  Translation      loads the French translation of the interface
  models/          StandModel, AlertModel
  commands/        APlanCommand; add/AddStandCommand, move/MoveStandCommand
  tools/           ITool, IToolContext, ATool, PointerEvent; select/SelectTool, stand/StandTool
qml/Mangrove/    Display-only QML modules, each one static:
  Theme/           Theme (tokens) and Icons (SVG paths) singletons + Inter and JetBrains Mono fonts, logo
  Controls/        generic building blocks, with no dependency on Editor:
                     Surface → Bar, SidePanel   (anything docked on an edge)
                     DockLayout                 (top / left / center / right / bottom)
                     TextLabel, Badge, Card, ModelList, Icon, IconToolButton
                     AppAction → ActionButton, ActionShortcuts
  Shell/           the editor: EditorActions (singleton, every command), ToolAction,
                   KeyboardShortcuts, TopBar, Toolbar, StatusBar, DocumentTitle, Workspace
  Panels/          side panel contents: AlertsPanel
qml/Main.qml     Says which bar goes on which edge of the DockLayout, nothing else.
src/main.cpp     Loads Main.qml and imports the static QML plugins.
tests/core/      Core tests (QtTest, QTEST_APPLESS_MAIN).
tests/app/       Tests of the Editor driven the way QML drives it (without loading any QML).
tests/ui/        End-to-end tests of the real window (bars, shortcuts), offscreen.
tests/fuzz/      libFuzzer targets (-DMANGROVE_FUZZ=ON, Clang).
tests/i18n/      Checks that every displayed text has its French translation.
translations/    mangrove_fr.ts: the French translation of every displayed text.
Documentation/   One page per source file, in the same tree (Documentation/README.md),
                 and the guides in Documentation/guides/: TESTING.md (running and writing
                 tests), QML_BUTTON.md and APP_BUTTON.md (adding a button), TRANSLATIONS.md
                 (French interface), ALERTS.md (plugging in the constraint algorithm).
```

For now the interface is only the bars: top bar (document, undo / redo), toolbar on the left,
alerts panel on the right, status bar at the bottom. The central `Workspace` is empty; the plan
canvas will be shown there. The whole C++ chain below already exists and is tested (`tests/app/`).

Path of a click: canvas → `Editor.press(pixels)` → `ViewTransform::toMeters` →
`ITool::press(IToolContext&, PointerEvent in meters)` → `IToolContext::addStand/moveStand` →
`APlanCommand` pushed onto the `QUndoStack` → `PlanDocument` emits its signals →
`StandModel` updates the matching row.

## Interfaces and abstract classes

Every extension point follows the same pattern:

- `I<Name>`: **interface**. Only pure virtual methods and a virtual destructor. This is the type the
  rest of the code works with.
- `A<Name>`: **abstract class** that implements the shared part of the interface (default behavior,
  shared helpers). Protected constructor: it is never instantiated.
- Concrete classes derive from `A<Name>` and live in a subfolder; the interface and the abstract
  class stay at the root of the folder.

| Extension      | Interface        | Abstract class   | What the base provides                                    | Concrete classes                 |
|----------------|------------------|------------------|-----------------------------------------------------------|----------------------------------|
| Tool           | `ITool`          | `ATool`          | `move`/`release`/`cancel` do nothing by default, `snap()` | `SelectTool`, `StandTool`        |
| Tool context   | `IToolContext`   | —                | what a tool can read and change                           | `Editor`                         |
| Command        | `QUndoCommand`   | `APlanCommand`   | access to the mutations of `PlanDocument`                 | `AddStandCommand`, `MoveStandCommand` |
| Edge (QML)     | —                | `Surface`        | background, thickness, separator line on the center side   | `Bar`, `SidePanel`               |
| Bar (QML)      | —                | `Bar`            | `leading` (start) and `trailing` (end) items, paddings    | `TopBar`, `Toolbar`, `StatusBar` |
| Panel (QML)    | —                | `SidePanel`      | title, counter, content area                              | `AlertsPanel`                    |
| Action (QML)   | —                | `AppAction`      | icon, text, shortcuts, state; `trigger()`                 | `ToolAction`, the actions of `EditorActions` |

On the QML side, a command is declared only once, in `EditorActions.qml`: `ActionButton` turns it
into a button wherever it is placed, and `KeyboardShortcuts` binds its shortcuts. Bars therefore
hold no logic: they place `ActionButton`s and `TextLabel`s.

## Rules

1. **No business logic in QML.** QML displays things and forwards events to `Editor`.
2. **The core works in meters.** Only `ViewTransform` converts meters ↔ pixels; no hard-coded
   scale factor anywhere else.
3. **Every change to the plan goes through a `QUndoCommand`** pushed onto the `QUndoStack`. The
   mutation methods of `PlanDocument` are only reachable from `APlanCommand`, the base class of
   commands.
4. **One tool = one class that derives from `ATool`.** Tools only see `IToolContext` (never
   `Editor` nor QML).
5. **An alert carries no text in the core.** An `Alert` holds a rule id and the stands involved;
   the translatable message (`tr()`) is written in `AlertModel`.
6. **Colors, spacings, radii, strokes and fonts come from `Theme.qml`.** No hard-coded values in
   the other QML files. All text is a `TextLabel` (never a raw `Text`).
7. **QML registration through macros**: `QML_ELEMENT`, `QML_SINGLETON`, `QML_UNCREATABLE`. Never
   `qmlRegisterType` nor `setContextProperty`.
8. **Every displayed text is written in English in the code**, inside `qsTr()` (QML) or `tr()`
   (C++), **and translated into French** in `translations/mangrove_fr.ts`. Never French in the code
   ([Documentation/guides/TRANSLATIONS.md](Documentation/guides/TRANSLATIONS.md)).

Other conventions: the core compiles with `-Wconversion -Wshadow -Wold-style-cast`; the build must
stay warning-free (`-DMANGROVE_WARNINGS_AS_ERRORS=ON` checks it in CI). Formatting:
`clang-format -i` (config in `.clang-format`; `src/app/.clang-format` overrides it for the app
layer: indented namespaces and `public:`/`private:`). clangd uses the same files in the IDE.

## Where do I add X?

| I want to add…                           | Where                                                                                                   |
|------------------------------------------|---------------------------------------------------------------------------------------------------------|
| A piece of plan data (aisle, wall, zone…) | `src/core/plan/` (+ tests in `tests/core/`)                                                            |
| A geometric primitive                     | `src/core/geometry/Geometry.h`                                                                          |
| Alerts from the constraint algorithm      | `Editor::setAlerts()`; the app checks nothing itself — see [Documentation/guides/ALERTS.md](Documentation/guides/ALERTS.md)              |
| An undoable action                        | `src/app/commands/<name>/` (derive from `APlanCommand`; merge id in `CommandIds.hpp`)                  |
| A document mutation                       | `src/app/PlanDocument.{hpp,cpp}` (private, + accessor in `APlanCommand.hpp`)                            |
| A tool                                    | `src/app/tools/<name>/` (class derived from `ATool`) + a value in `Editor::ToolKind`, registration in the `Editor` constructor and a `ToolAction` in `qml/Mangrove/Shell/EditorActions.qml` |
| A command (button and/or shortcut)        | an `AppAction` in `qml/Mangrove/Shell/EditorActions.qml`, then an `ActionButton` in the bar you want — see [Documentation/guides/QML_BUTTON.md](Documentation/guides/QML_BUTTON.md) and [Documentation/guides/APP_BUTTON.md](Documentation/guides/APP_BUTTON.md) |
| Data exposed to QML                       | `src/app/models/` or a property of `Editor`                                                             |
| A zoom / pan setting                      | `src/app/ViewTransform.{hpp,cpp}`                                                                       |
| A color, a size, a font, the logo         | `qml/Mangrove/Theme/Theme.qml` (the logo file is in `qml/Mangrove/Theme/images/`)                       |
| A reusable component                      | `qml/Mangrove/Controls/` (with no dependency on `Editor`)                                               |
| An icon                                   | `qml/Mangrove/Theme/Icons.qml` (an SVG path on a 24 × 24 grid)                                          |
| A text shown to the user                  | `qsTr()` / `tr()` in English, then its French translation in `translations/mangrove_fr.ts` — see [Documentation/guides/TRANSLATIONS.md](Documentation/guides/TRANSLATIONS.md) |
| A text style                              | `qml/Mangrove/Controls/TextLabel.qml` (a value of `TextLabel.Style`)                                    |
| The plan canvas                           | a new QML module (see below), shown as the content of `qml/Mangrove/Shell/Workspace.qml`                |
| A bar                                     | `qml/Mangrove/Shell/` (root `Bar`, with its `edge`) then dock it in the `DockLayout` of `qml/Main.qml`  |
| A side panel                              | `qml/Mangrove/Panels/` (root `SidePanel`) then dock it in the `DockLayout` of `qml/Main.qml`            |
| A keyboard shortcut                       | the `shortcuts` field of the matching action in `qml/Mangrove/Shell/EditorActions.qml`                  |
| A new QML module                          | `qml/Mangrove/<Name>/CMakeLists.txt` + `add_subdirectory` in `qml/CMakeLists.txt` + link and `Q_IMPORT_QML_PLUGIN` in `CMakeLists.txt` / `src/main.cpp` |
| Any source file                           | its page in `Documentation/`, at the same path (`src/app/Editor.cpp` → `Documentation/src/app/Editor.md`) |
| A test                                    | `tests/core/`, `tests/app/`, `tests/ui/`, `tests/i18n/` or `tests/fuzz/` — see [Documentation/guides/TESTING.md](Documentation/guides/TESTING.md)      |
