# Mangrove Project
# tests/core: unit tests of the pure C++ core, run with QtTest (no QApplication).

mangrove_add_test(tst_geometry LIBS Mangrove::Core)
mangrove_add_test(tst_plan LIBS Mangrove::Core)
mangrove_add_test(tst_core_robustness LIBS Mangrove::Core LABELS security)
mangrove_add_test(tst_plan_invariants LIBS Mangrove::Core LABELS security)
