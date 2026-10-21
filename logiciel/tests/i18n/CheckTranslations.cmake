# Mangrove Project
# Fails if a .ts file still holds texts without a translation, and lists them.
# Usage: cmake -DTS_FILE=<file.ts> -P CheckTranslations.cmake

file(READ "${TS_FILE}" content)
string(REGEX MATCHALL "<source>[^<]*</source>[ \t\r\n]*<translation type=\"unfinished\""
    unfinished "${content}")

if(unfinished)
    set(report "")
    foreach(entry IN LISTS unfinished)
        string(REGEX REPLACE "<source>([^<]*)</source>.*" "\\1" text "${entry}")
        string(APPEND report "\n  - ${text}")
    endforeach()
    message(FATAL_ERROR "Texts without a French translation in ${TS_FILE}:${report}\n"
        "Translate them in that file (see Documentation/guides/TRANSLATIONS.md).")
endif()
message(STATUS "Every text of ${TS_FILE} is translated.")
