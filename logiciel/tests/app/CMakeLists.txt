# Mangrove Project
# tests/app: drives the QML bridge from C++, without loading any QML.

mangrove_add_test(tst_editor LIBS Mangrove::App)
mangrove_add_test(tst_app_robustness LIBS Mangrove::App LABELS security)
mangrove_add_test(tst_undo_invariants LIBS Mangrove::App LABELS security)
