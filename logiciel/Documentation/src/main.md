# main

> Executable — [`main.cpp`](../../src/main.cpp)

Application entry point: creates the Qt application, loads the QML window and runs the event loop.

## How it works

1. Imports the five static QML modules with `Q_IMPORT_QML_PLUGIN`: `App`, `Theme`, `Controls`,
   `Shell`, `Panels`. They are linked as static libraries: without this line, the QML engine
   cannot find them.
2. Creates the `QGuiApplication` and sets its name, organization and version. The version
   (`MANGROVE_VERSION`) comes from CMake's `project(VERSION)` and is shown in the status bar.
3. Installs the French translation ([Translation](app/Translation.md)) **before** loading any QML,
   so that every `qsTr()` is translated.
4. Creates the `QQmlApplicationEngine` and loads `Main` from the `Mangrove.Main` module.
5. If the window cannot be created (QML error), the application exits with `EXIT_FAILURE`.

## Links

- Loads [Main](../qml/Main.md).
- C++ types visible in QML (`Editor`, the models…) register themselves through macros
  (`QML_ELEMENT`, `QML_SINGLETON`): nothing to declare here.

## Extending it

New QML module: one `Q_IMPORT_QML_PLUGIN(Mangrove_<Name>Plugin)` line here, plus the
`mangrove_<name> mangrove_<name>plugin` link in `CMakeLists.txt`.

---
[↑ Contents](../README.md)
