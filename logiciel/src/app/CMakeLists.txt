# Mangrove Project
# src/app: QML module "Mangrove.App", the bridge between the core and QML.

qt_add_qml_module(mangrove_app
    URI Mangrove.App
    VERSION 1.0
    STATIC
    SOURCES
        Conversions.hpp
        Editor.hpp Editor.cpp
        PlanDocument.hpp PlanDocument.cpp
        Translation.hpp Translation.cpp
        ViewTransform.hpp ViewTransform.cpp
        models/stand/StandModel.hpp models/stand/StandModel.cpp
        models/alert/AlertModel.hpp models/alert/AlertModel.cpp
        commands/CommandIds.hpp
        commands/APlanCommand.hpp commands/APlanCommand.cpp
        commands/add/AddStandCommand.hpp commands/add/AddStandCommand.cpp
        commands/move/MoveStandCommand.hpp commands/move/MoveStandCommand.cpp
        tools/PointerEvent.hpp
        tools/IToolContext.hpp
        tools/ITool.hpp
        tools/ATool.hpp tools/ATool.cpp
        tools/select/SelectTool.hpp tools/select/SelectTool.cpp
        tools/stand/StandTool.hpp tools/stand/StandTool.cpp
)
add_library(Mangrove::App ALIAS mangrove_app)

target_include_directories(mangrove_app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mangrove_app PUBLIC Mangrove::Core Qt6::Core Qt6::Gui Qt6::Qml)
mangrove_enable_warnings(mangrove_app)
