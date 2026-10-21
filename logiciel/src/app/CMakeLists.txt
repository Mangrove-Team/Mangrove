# Mangrove Project
# src/app: QML module "Mangrove.App", the bridge between the core and QML.

qt_add_qml_module(mangrove_app
    URI Mangrove.App
    VERSION 1.0
    STATIC
    SOURCES
        Editor.cpp
        Translation.cpp
        PlanDocument.cpp
        ViewTransform.cpp

        Editor.hpp
        Translation.hpp
        Conversions.hpp
        PlanDocument.hpp
        ViewTransform.hpp

        models/stand/StandModel.cpp
        models/alert/AlertModel.cpp
        models/stand/StandModel.hpp
        models/alert/AlertModel.hpp

        commands/APlanCommand.cpp
        commands/add/AddStandCommand.cpp
        commands/move/MoveStandCommand.cpp

        commands/CommandIds.hpp
        commands/APlanCommand.hpp
        commands/add/AddStandCommand.hpp
        commands/move/MoveStandCommand.hpp

        tools/ATool.cpp
        tools/stand/StandTool.cpp
        tools/select/SelectTool.cpp

        tools/ITool.hpp
        tools/ATool.hpp
        tools/PointerEvent.hpp
        tools/IToolContext.hpp
        tools/stand/StandTool.hpp
        tools/select/SelectTool.hpp
)
add_library(Mangrove::App ALIAS mangrove_app)

target_include_directories(mangrove_app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(mangrove_app PUBLIC Mangrove::Core Qt6::Core Qt6::Gui Qt6::Qml)
mangrove_enable_warnings(mangrove_app)
