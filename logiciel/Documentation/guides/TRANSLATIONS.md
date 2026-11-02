# Translations — Mangrove desktop app

The code is written in English, but the application is shown to its users **in French**. Every text
shown to the user is therefore written in English in the code and translated into French in
[`translations/mangrove_fr.ts`](../../translations/mangrove_fr.ts).

## 1. How it works

```
qsTr("Undo") in QML, tr("Add stand %1") in C++        English source text
        │  cmake --build build --target update_translations   (lupdate)
        ▼
translations/mangrove_fr.ts                             one <message> per text, translated by hand
        │  any build                                          (lrelease)
        ▼
:/i18n/mangrove_fr.qm, embedded in mangrove_app
        │  loadTranslation() + installTranslator() in main.cpp, before the QML is loaded
        ▼
"Annuler", "Ajouter le stand S03"                       what the user sees
```

- `qt_add_translations()` in the root [`CMakeLists.txt`](../../CMakeLists.txt) scans every target
  (`Mangrove`, `mangrove_app`, `mangrove_controls`, `mangrove_shell`, `mangrove_panels`).
- The `.ts` file is **committed**; the `.qm` file is generated at build time and ignored by git.
- [`Translation.hpp`](../../src/app/Translation.hpp) loads the `.qm`. The app and the tests use the
  same function, so the tests check exactly what ships.

## 2. Adding or changing a text

1. Write the text in English, inside `qsTr()` in QML or `tr()` in C++:

   ```qml
   text: qsTr("Delete")
   ```

   ```cpp
   setText(tr("Remove stand %1").arg(toQt(_stand.label)));
   ```

   A C++ class that uses `tr()` needs `Q_OBJECT` or `Q_DECLARE_TR_FUNCTIONS(ClassName)`.

2. Update the `.ts` file:

   ```sh
   cmake --build build --target update_translations
   ```

   The new text appears in `translations/mangrove_fr.ts` as `type="unfinished"`.

3. Translate it, with Qt Linguist (`linguist translations/mangrove_fr.ts`) or by hand:

   ```xml
   <message>
       <source>Delete</source>
       <translation>Supprimer</translation>
   </message>
   ```

   Remove `type="unfinished"` once the text is translated (Linguist does it for you).

4. Rebuild and check: `ctest --test-dir build -L i18n`.

5. Commit the `.ts` file **with** the code that adds the text.

## 3. Rules

- **Never write French in the code**, and never concatenate a displayed sentence: use `%1`, `%2`
  and `arg()`, because French word order differs from English.
- With several arguments, use a single call, `arg(a, b)`: a label containing `%1` is then shown
  as is (see the security checklist in [TESTING.md](TESTING.md)).
- Keep `%1`, `%2` and keyboard shortcuts (`Ctrl+Z`) unchanged in the translation.
- If the same English word needs two different French translations (for example "Cancel" for a
  gesture and for a dialog), add a disambiguation comment: `qsTr("Cancel", "gesture")`.

## 4. Tests

| Test | What it checks |
|---|---|
| `i18n.translationsAreComplete` | no text of `mangrove_fr.ts` is left untranslated |
| `ui.tst_ui::interfaceIsInFrench` | with the translation installed, the real window and the C++ messages are in French |

The other `tst_ui` tests run without a translator, so they compare the English source texts.

---

[Project README](../../README.md) · [Testing guide](TESTING.md)
