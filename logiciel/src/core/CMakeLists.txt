# Mangrove Project
# src/core: pure C++20 domain library. No framework dependency, ever.

add_library(mangrove_core STATIC
    geometry/Geometry.h
    plan/Ids.h
    plan/Stand.h
    plan/Plan.h
    plan/Plan.cpp
    constraints/Alert.h
)
add_library(Mangrove::Core ALIAS mangrove_core)

target_include_directories(mangrove_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(mangrove_core PUBLIC cxx_std_20)
# No moc/uic/rcc: nothing to generate here.
set_target_properties(mangrove_core PROPERTIES AUTOMOC OFF AUTOUIC OFF AUTORCC OFF)
mangrove_enable_warnings(mangrove_core STRICT)
