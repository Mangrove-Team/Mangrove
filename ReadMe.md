# Mangrove

Mangrove is a desktop floor-plan editor built with Qt 6 (QML + C++). It lets you manage exhibitors/stands and edit plan elements through a visual canvas and toolbar UI.

## Requirements

- CMake >= 3.21
- A C++23 compiler
- Qt 6 (`Core`, `Gui`, `Quick`, `Qml`, `Svg`)
- vcpkg (toolchain integration is used by existing build scripts)

## Build and run

### Linux

The provided script compiles into `build/` and copies the binary to the repository root.

```bash
chmod +x linux_build.sh
./linux_build.sh
./Mangrove
```

> `linux_build.sh` contains a machine-specific `CMAKE_TOOLCHAIN_FILE` path. Update it to your local vcpkg path before running.

### Windows

```bat
windows_build.bat
```

This script:
- configures CMake in `build/`
- builds `Release`
- generates a package with CPack

> `windows_build.bat` also contains a machine-specific vcpkg toolchain path and generator. Update them for your environment if needed.

## Clean build artifacts

### Linux

```bash
chmod +x linux_clean.sh
./linux_clean.sh
```

### Windows

```bat
windows_clean.bat
```

## Project layout

- `src/`: C++ sources (`Plan`, `Stand`, constraints, drawing backend, app entrypoint)
- `src/style/`: QML views/components (`Main.qml`, toolbar, map, popup, etc.)
- `resources/`: runtime assets copied next to the executable after build
- `icons/`: SVG icon assets
- `mobile/PrototypeMobile/`: separate mobile prototype
- `resources.qrc`: Qt resource file

## Troubleshooting

- If CMake cannot find Qt (`find_package(Qt6 ...)`):
  - verify Qt 6 is installed with required modules
  - ensure your environment/toolchain points to the correct Qt installation
- If configuration fails on toolchain:
  - update the hardcoded `CMAKE_TOOLCHAIN_FILE` path in build scripts
- If the app starts but resources are missing:
  - confirm `resources/` was copied next to the built executable
