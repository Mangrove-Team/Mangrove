cmake_minimum_required(VERSION 3.25)

# Must run before project(): optionally picks the vcpkg toolchain that installs Qt.
include(cmake/VcpkgBootstrap.cmake)

project(Mangrove
        VERSION 0.1.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(MANGROVE_BUILD_TESTS "Build the unit tests" ON)

find_package(Qt6 6.8 REQUIRED COMPONENTS Core Gui Qml Quick Test LinguistTools)
qt_standard_project_setup(REQUIRES 6.8)

# Every QML module lands in build/qml/<URI path>, where qmllint and the tooling find it.
set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(CompilerWarnings)
include(Sanitizers)
include(GNUInstallDirs)

# ---------------------------------------------------------------------------
# Layers: core (pure C++) -> app (QML bridge) -> qml (display only)
# ---------------------------------------------------------------------------
add_subdirectory(src/core)
add_subdirectory(src/app)
add_subdirectory(qml)

# ---------------------------------------------------------------------------
# Executable: main.cpp + Main.qml, which only assembles the modules
# ---------------------------------------------------------------------------
qt_add_executable(Mangrove
        src/main.cpp
)

set_source_files_properties(qml/Main.qml PROPERTIES QT_RESOURCE_ALIAS Main.qml)
qt_add_qml_module(Mangrove
        URI Mangrove.Main
        VERSION 1.0
        QML_FILES
        qml/Main.qml
)

set_target_properties(Mangrove PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE TRUE
)

# Static QML modules: link each backing library and its plugin
# (imported in main.cpp with Q_IMPORT_QML_PLUGIN).
target_link_libraries(Mangrove PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        mangrove_app mangrove_appplugin
        mangrove_theme mangrove_themeplugin
        mangrove_controls mangrove_controlsplugin
        mangrove_shell mangrove_shellplugin
        mangrove_panels mangrove_panelsplugin
)
target_compile_definitions(Mangrove PRIVATE MANGROVE_VERSION="${PROJECT_VERSION}")
mangrove_enable_warnings(Mangrove)

# ---------------------------------------------------------------------------
# Translations: texts are written in English in the code and shown in French.
# The .qm is embedded in mangrove_app (":/i18n/mangrove_fr.qm"), so the app and
# the tests load the same file. After adding or changing a text, run
#   cmake --build build --target update_translations
# then translate it in translations/mangrove_fr.ts (see Documentation/guides/TRANSLATIONS.md).
# ---------------------------------------------------------------------------
qt_add_translations(
    TARGETS mangrove_app
    SOURCE_TARGETS Mangrove mangrove_app mangrove_controls mangrove_shell mangrove_panels
    TS_FILES translations/mangrove_fr.ts
    LUPDATE_OPTIONS -no-obsolete -locations none
)

if(MANGROVE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ---------------------------------------------------------------------------
# Install: binary + every Qt .dll/.so, platform plugins and QML modules it uses
# ---------------------------------------------------------------------------
install(TARGETS Mangrove
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Only bundle Qt when it comes from vcpkg: a system Qt is already installed
# on the machine and is used as-is.
if(MANGROVE_USE_VCPKG)
    set(mangrove_deploy_options "")
    if(UNIX AND NOT APPLE)
        # Never ship system libraries (glibc, ld-linux, libGL, X11...):
        # only what comes from vcpkg_installed/ is bundled.
        list(APPEND mangrove_deploy_options
                POST_EXCLUDE_REGEXES "^/lib" "^/usr/lib" "^/usr/local/lib")
    endif()

    qt_generate_deploy_qml_app_script(
            TARGET Mangrove
            OUTPUT_SCRIPT mangrove_deploy_script
            ${mangrove_deploy_options}
    )
    install(SCRIPT ${mangrove_deploy_script})
endif()

# ---------------------------------------------------------------------------
# Packaging (cpack --preset <name>)
# ---------------------------------------------------------------------------
set(CPACK_PACKAGE_NAME "Mangrove")
set(CPACK_PACKAGE_VENDOR "Mangrove")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
if(WIN32)
    set(CPACK_GENERATOR "NSIS;ZIP")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
else()
    set(CPACK_GENERATOR "TGZ")
endif()
include(CPack)
