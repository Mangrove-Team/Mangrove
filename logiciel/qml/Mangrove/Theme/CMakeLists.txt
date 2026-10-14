# Mangrove Project
# QML module "Mangrove.Theme": design tokens, icons and bundled fonts.

set_source_files_properties(Theme.qml Icons.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(mangrove_theme
    URI Mangrove.Theme
    VERSION 1.0
    STATIC
    QML_FILES
        Theme.qml
        Icons.qml
    RESOURCES
        fonts/IBMPlexSans.ttf
        fonts/IBMPlexMono-Regular.ttf
        fonts/IBMPlexMono-Medium.ttf
        fonts/IBMPlexMono-SemiBold.ttf
)

target_link_libraries(mangrove_theme PRIVATE Qt6::Quick)
