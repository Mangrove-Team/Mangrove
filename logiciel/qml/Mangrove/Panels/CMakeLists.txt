# Mangrove Project
# QML module "Mangrove.Panels": side panels around the canvas.

qt_add_qml_module(mangrove_panels
    URI Mangrove.Panels
    VERSION 1.0
    STATIC
    QML_FILES
        AlertsPanel.qml
)

# mangrove_app: compiled QML includes the headers of the C++ types it uses.
target_link_libraries(mangrove_panels PRIVATE
    Qt6::Quick mangrove_app mangrove_theme mangrove_controls)
