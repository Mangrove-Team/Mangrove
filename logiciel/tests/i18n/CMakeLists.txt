# Mangrove Project
# tests/i18n: every text shown to the user has its French translation.

add_test(NAME i18n.translationsAreComplete
    COMMAND ${CMAKE_COMMAND}
        -DTS_FILE=${PROJECT_SOURCE_DIR}/translations/mangrove_fr.ts
        -P ${CMAKE_CURRENT_SOURCE_DIR}/CheckTranslations.cmake)
set_tests_properties(i18n.translationsAreComplete PROPERTIES LABELS i18n)
