# Translation

> App layer · QML module `Mangrove.App` — [`Translation.hpp`](../../../src/app/Translation.hpp) · [`Translation.cpp`](../../../src/app/Translation.cpp)

Loads the French translation of the interface. Texts are written in English in the code and shown in French.

## API

| Function | What it does |
|---|---|
| `loadTranslation()` | the French translation (`:/i18n/mangrove_fr.qm`) as a `QTranslator`, or null if it cannot be loaded |

## How it works

- `qt_add_translations()` (root `CMakeLists.txt`) compiles `translations/mangrove_fr.ts` into
  `mangrove_fr.qm` and embeds it in `mangrove_app`: the app and the tests load the same file.
- [main](../main.md) installs it with `QCoreApplication::installTranslator()` before loading the
  QML. The translator uninstalls itself when destroyed.

## Caveats

- A text missing from the `.ts` file, or left untranslated, is shown in English: run
  `update_translations` after adding a text (see [guides/TRANSLATIONS.md](../../guides/TRANSLATIONS.md)).
- Installing a translator after the QML is loaded needs `QQmlEngine::retranslate()` to update the
  texts already shown (`tst_ui::interfaceIsInFrench` does this).

## Links

Used by [main](../main.md); tested by `tests/ui/tst_ui.cpp` and `tests/i18n/`.

---
[↑ Contents](../../README.md)
