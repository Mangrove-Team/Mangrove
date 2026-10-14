# Mangrove Project
# QML module "Mangrove.Controls": generic, editor-agnostic building blocks.

qt_add_qml_module(mangrove_controls
    URI Mangrove.Controls
    VERSION 1.0
    STATIC
    QML_FILES
        # Docked on the window edges
        Surface.qml
        Bar.qml
        SidePanel.qml
        DockLayout.qml
        # Content
        TextLabel.qml
        Badge.qml
        Card.qml
        ModelList.qml
        Icon.qml
        IconToolButton.qml
        # Actions
        AppAction.qml
        ActionButton.qml
        ActionShortcuts.qml
)

target_link_libraries(mangrove_controls PRIVATE Qt6::Quick mangrove_theme)
