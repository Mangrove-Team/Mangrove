# Mangrove Project
# QML module "Mangrove.Shell": the editor's actions and the bars that show them.

set_source_files_properties(EditorActions.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(mangrove_shell
    URI Mangrove.Shell
    VERSION 1.0
    STATIC
    QML_FILES
        # Actions
        ToolAction.qml
        EditorActions.qml
        KeyboardShortcuts.qml
        # Bars and center
        DocumentTitle.qml
        TopBar.qml
        Toolbar.qml
        StatusBar.qml
        Workspace.qml
        Ruler.qml
)

# mangrove_app: compiled QML includes the headers of the C++ types it uses.
target_link_libraries(mangrove_shell PRIVATE
    Qt6::Quick mangrove_app mangrove_theme mangrove_controls)
