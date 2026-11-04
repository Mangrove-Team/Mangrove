# Mangrove Project
# tests/ui: end-to-end tests of the real window, rendered offscreen.

mangrove_add_test(tst_ui
    LIBS
        Qt6::Quick
        mangrove_app mangrove_appplugin
        mangrove_theme mangrove_themeplugin
        mangrove_controls mangrove_controlsplugin
        mangrove_shell mangrove_shellplugin
        mangrove_panels mangrove_panelsplugin
    ENVIRONMENT
        QT_QPA_PLATFORM=offscreen
        QT_QUICK_BACKEND=software
)
# Main.qml is loaded from the sources: it belongs to the executable, not to a library.
target_compile_definitions(tst_ui PRIVATE MANGROVE_MAIN_QML="${PROJECT_SOURCE_DIR}/qml/Main.qml")
