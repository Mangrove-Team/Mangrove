# Mangrove Project
# tests: one sub-directory per layer under test. See Documentation/guides/TESTING.md.

# mangrove_add_test(<name> [LIBS <targets...>] [LABELS <labels...>] [ENVIRONMENT <VAR=value...>])
#   Builds <name>.cpp from the current directory into a QtTest executable and
#   registers it as "<directory>.<name>" with the directory name as first label,
#   so `ctest -L core`, `ctest -L security`... select tests.
function(mangrove_add_test name)
    cmake_parse_arguments(PARSE_ARGV 1 arg "" "" "LIBS;LABELS;ENVIRONMENT")
    get_filename_component(layer ${CMAKE_CURRENT_SOURCE_DIR} NAME)

    qt_add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE Qt6::Test ${arg_LIBS})
    target_include_directories(${name} PRIVATE ${PROJECT_SOURCE_DIR}/tests/support)
    mangrove_enable_warnings(${name})

    set(environment ${arg_ENVIRONMENT})
    if(MANGROVE_SANITIZERS)
        list(APPEND environment
            "LSAN_OPTIONS=suppressions=${PROJECT_SOURCE_DIR}/tests/sanitizers/lsan.supp"
            "UBSAN_OPTIONS=print_stacktrace=1")
    endif()

    add_test(NAME ${layer}.${name} COMMAND ${name})
    set_tests_properties(${layer}.${name} PROPERTIES
        LABELS "${layer};${arg_LABELS}"
        ENVIRONMENT "${environment}"
        TIMEOUT 300)
endfunction()

add_subdirectory(core)
add_subdirectory(app)
add_subdirectory(ui)
add_subdirectory(i18n)

if(MANGROVE_FUZZ)
    add_subdirectory(fuzz)
endif()
