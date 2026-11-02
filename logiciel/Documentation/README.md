# Documentation — Mangrove desktop app

One page per source file, stored in the same tree as the code:
`src/app/Editor.cpp` → [`src/app/Editor.md`](src/app/Editor.md).

- A `.hpp` / `.cpp` (or `.h` / `.cpp`) pair shares one page, named after the class.
- Files without a `.cpp` (interfaces, header-only files) have their own page too.
- Every `.qml` file has its page.

For the big picture (layers, rules, "Where do I add X?"), read the
[project README](../README.md) first. Tests are described in [guides/TESTING.md](guides/TESTING.md).

## Structure of a page

1. Header: the layer and links to the source files.
2. The role, in one or two sentences.
3. Depending on the file: *How it works*, *API* or *Properties*, *Caveats*, *Extending it*,
   *Links* to the neighboring pages.

## Conventions

| Prefix | Meaning | Examples |
|---|---|---|
| `I…` | interface: only pure virtual methods | `ITool`, `IToolContext` |
| `A…` | abstract class: the shared part of an interface | `ATool`, `APlanCommand` |

In QML, the bases to inherit from are `Surface`, `Bar`, `SidePanel` and `AppAction`.

## Guides

- [TESTING](guides/TESTING.md) — Running the tests, and which test to write for which change
- [QML_BUTTON](guides/QML_BUTTON.md) — Adding a button whose action `Editor` can already do
- [APP_BUTTON](guides/APP_BUTTON.md) — Adding a button that needs new C++ in `Editor`
- [TRANSLATIONS](guides/TRANSLATIONS.md) — Translating the interface into French
- [ALERTS](guides/ALERTS.md) — Plugging the constraint algorithm into the app

## Contents

- **src/**
  - [main](src/main.md) — Application entry point: creates the Qt application, loads the QML window and runs the event loop
  - **core/**
    - **geometry/**
      - [Geometry](src/core/geometry/Geometry.md) — Geometric primitives of the plan, in meters: `Vec2` (point or vector) and `RectM` (axis-aligned rectangle, y pointing down as on screen)
    - **plan/**
      - [Ids](src/core/plan/Ids.md) — Strong identifier of a stand, `StandId`: a wrapped `uint64_t` so it is never mistaken for an index or another number
      - [Stand](src/core/plan/Stand.md) — A stand placed on the plan: an id, a displayed label ("S01") and a rectangle in meters
      - [Plan](src/core/plan/Plan.md) — The plan of an event: the hall (60 × 40 m by default) and the stands placed in it
    - **constraints/**
      - [Alert](src/core/constraints/Alert.md) — A constraint violation: the id of the rule that raised it (`ruleId`) and the stands involved
  - **app/**
    - [Editor](src/app/Editor.md) — The single entry point from QML into the application, exposed as the `Editor` QML singleton
    - [PlanDocument](src/app/PlanDocument.md) — Owns the plan and its undo history (`QUndoStack`)
    - [ViewTransform](src/app/ViewTransform.md) — The only place where meters are converted to screen pixels and back: `screen = meters × scale + offset`, with `scale = pixelsPerMeter × zoom`
    - [Translation](src/app/Translation.md) — Loads the French translation of the interface, embedded in the app as `:/i18n/mangrove_fr.qm`
    - [Conversions](src/app/Conversions.md) — Value conversions between core types and Qt types: `Vec2` ↔ `QPointF`, `RectM` ↔ `QRectF`, `std::string` → `QString`
    - **models/**
      - **stand/**
        - [StandModel](src/app/models/stand/StandModel.md) — List model (`QAbstractListModel`) of the stands for QML, in meters
      - **alert/**
        - [AlertModel](src/app/models/alert/AlertModel.md) — List model of the constraint alerts for QML (`Editor.alerts`)
    - **commands/**
      - [CommandIds](src/app/commands/CommandIds.md) — Values of `QUndoCommand::id()`
      - [APlanCommand](src/app/commands/APlanCommand.md) — Abstract base class of **every** command that changes the plan
      - **add/**
        - [AddStandCommand](src/app/commands/add/AddStandCommand.md) — Adds a stand on top of the others
      - **move/**
        - [MoveStandCommand](src/app/commands/move/MoveStandCommand.md) — Moves a stand from a `from` rectangle to a `to` rectangle
    - **tools/**
      - [PointerEvent](src/app/tools/PointerEvent.md) — A pointer event as tools receive it: a position already converted to meters
      - [IToolContext](src/app/tools/IToolContext.md) — Interface of everything a tool can read and do
      - [ITool](src/app/tools/ITool.md) — Interface of an editing tool
      - [ATool](src/app/tools/ATool.md) — Abstract base class of tools
      - **select/**
        - [SelectTool](src/app/tools/select/SelectTool.md) — Select tool (V key): selects the stand under the pointer and drags it, snapped to the grid
      - **stand/**
        - [StandTool](src/app/tools/stand/StandTool.md) — Stand tool (R key): draws a new stand by dragging a rectangle snapped to the grid
- **qml/**
  - [Main](qml/Main.md) — Main window
  - **Mangrove/**
    - **Theme/**
      - [Theme](qml/Mangrove/Theme/Theme.md) — Singleton, single source of colors, spacings, radii, strokes, fonts, icon sizes and bar dimensions
      - [Icons](qml/Mangrove/Theme/Icons.md) — Singleton, icon catalog: for each name, an SVG path (the `d` attribute of a `<path>`) on a 24 × 24 grid, stroked by `Icon`
    - **Controls/**
      - [Surface](qml/Mangrove/Controls/Surface.md) — Base of anything docked on an edge of the window (bars, panels): a panel background, a thickness, and a separator line on the center side
      - [Bar](qml/Mangrove/Controls/Bar.md) — Base of every bar
      - [SidePanel](qml/Mangrove/Controls/SidePanel.md) — Base of side panels: a `Surface` docked on the right by default, `Theme.panelWidth` wide, with a title, a counter and a content area
      - [DockLayout](qml/Mangrove/Controls/DockLayout.md) — Window layout: up to four items on the edges, around a central item
      - [TextLabel](qml/Mangrove/Controls/TextLabel.md) — Every text of the interface
      - [Badge](qml/Mangrove/Controls/Badge.md) — Counter tag
      - [Card](qml/Mangrove/Controls/Card.md) — Rounded block holding a wrapping text; its height follows the text
      - [ModelList](qml/Mangrove/Controls/ModelList.md) — Vertical list of the rows of a model, or a message when the model is empty
      - [Icon](qml/Mangrove/Controls/Icon.md) — Line icon drawn from the `Icons` paths: crisp at any size and in any color
      - [IconToolButton](qml/Mangrove/Controls/IconToolButton.md) — Square button showing an icon
      - [AppAction](qml/Mangrove/Controls/AppAction.md) — A user command, independent of how it is shown: `ActionButton` turns it into a button, `ActionShortcuts` into keyboard shortcuts
      - [ActionButton](qml/Mangrove/Controls/ActionButton.md) — `IconToolButton` driven by an `AppAction`: icon, tooltip, checked state, enabled state and click come from the action
      - [ActionShortcuts](qml/Mangrove/Controls/ActionShortcuts.md) — Binds the keyboard shortcuts of a list of actions in the window it lives in
    - **Shell/**
      - [ToolAction](qml/Mangrove/Shell/ToolAction.md) — An `AppAction` that activates an editor tool: it is `active` when `Editor.activeTool` equals its `kind`, and triggering it makes `kind` the active tool
      - [EditorActions](qml/Mangrove/Shell/EditorActions.md) — Singleton, registry of every editor command
      - [KeyboardShortcuts](qml/Mangrove/Shell/KeyboardShortcuts.md) — Binds the shortcuts of every action in `EditorActions` (tools and commands), whatever bars are shown: removing the toolbar does not remove V or R
      - [DocumentTitle](qml/Mangrove/Shell/DocumentTitle.md) — "*logo* / *document name*", as shown in the top bar
      - [TopBar](qml/Mangrove/Shell/TopBar.md) — Top bar: the document title on the left, the Undo / Redo buttons on the right
      - [Toolbar](qml/Mangrove/Shell/Toolbar.md) — Vertical bar on the left: one button per tool of `EditorActions.tools`, in order
      - [StatusBar](qml/Mangrove/Shell/StatusBar.md) — Bottom bar: the name of the active tool on the left, the application version on the right
      - [Workspace](qml/Mangrove/Shell/Workspace.md) — Central area, between the bars
      - [Ruler](qml/Mangrove/Shell/Ruler.md) — Graduated bar along one edge of the plan, in meters, following the zoom and the pan of `Editor.view`
    - **Panels/**
      - [AlertsPanel](qml/Mangrove/Panels/AlertsPanel.md) — Side panel of constraint violations: an "Alerts" title with a counter, then one card per alert, or "No issues on this plan."

## Keeping the docs up to date

Adding, renaming or deleting a source file → do the same with its page, in the same commit.
