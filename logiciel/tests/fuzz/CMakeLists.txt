# Mangrove Project
# tests/fuzz: libFuzzer targets (configure with -DMANGROVE_FUZZ=ON and Clang).
# ctest runs each target for a fixed number of iterations as a smoke test;
# run the binary directly to fuzz for longer (see Documentation/guides/TESTING.md).

function(mangrove_add_fuzz_target name)
    cmake_parse_arguments(PARSE_ARGV 1 arg "" "" "LIBS")
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE ${arg_LIBS})
    target_link_options(${name} PRIVATE -fsanitize=fuzzer)
    set_target_properties(${name} PROPERTIES AUTOMOC OFF)

    add_test(NAME fuzz.${name} COMMAND ${name} -runs=200000 -seed=1)
    set_tests_properties(fuzz.${name} PROPERTIES LABELS "fuzz;security" TIMEOUT 300)
endfunction()

mangrove_add_fuzz_target(fuzz_plan LIBS Mangrove::Core)
