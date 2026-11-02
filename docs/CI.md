# CI : pipelines et écriture de tests

La CI tourne sur **GitHub Actions** (`.github/workflows/ci.yml`). Elle se lance à chaque push, sur toutes les branches, et sur chaque pull request.

## Ce que vérifie la pipeline

Deux jobs tournent en parallèle :

| Job | Étapes | Échoue si… |
|-----|--------|------------|
| **Format** | `pre-commit` sans clang-tidy | le code n'est pas formaté selon le `.clang-format` |
| **Build, tidy & test** | Installation de Qt 6.8 → configure CMake → build → clang-tidy → ctest | le projet ne compile pas, clang-tidy remonte une erreur, ou un test échoue |

Si un nouveau commit est poussé sur la même branche, le run précédent est annulé.

Une PR ne doit être mergée que si les deux jobs sont verts.

## Lire le résultat d'un run

Dans l'onglet **Actions** du dépôt (ou via les checks en bas d'une PR) :

- **Summary** du run : tableau de tous les tests avec leur statut et leur durée.
- **Logs d'un job** : cliquer sur l'étape en échec. Pour un test, la sortie complète du test en échec est affichée (`--output-on-failure`).
- **Format en échec** : le diff attendu est affiché dans les logs. Lancer `pre-commit run --all-files` en local corrige les fichiers automatiquement.

## Reproduire la CI en local

Avant de pousser, les mêmes commandes que la CI :

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
pre-commit run --all-files
QT_QPA_PLATFORM=offscreen ctest --test-dir build --output-on-failure
```

> Les commandes se lancent **depuis la racine** du dépôt (le `CMakeLists.txt` racine inclut `logiciel/`). Le dossier de build doit s'appeler **`build/`** : le hook clang-tidy lit `build/compile_commands.json`. Sous CLion, changer le dossier dans *Settings → Build, Execution, Deployment → CMake → Build directory*.

## Écrire un test C++ compatible

### Le principe

La CI lance **`ctest`**, qui exécute tous les tests déclarés dans CMake. Un test est simplement un **exécutable** :

- code de sortie **0** → test réussi ;
- tout autre code de sortie (ou un crash) → test échoué.

Un test qui n'est pas déclaré avec `add_test` n'est **pas lancé** par la CI, même s'il compile.

### Où placer les tests

Les tests de l'application sont dans `logiciel/tests/`, un sous-dossier par couche (`core/`, `app/`, `ui/`, `i18n/`, `fuzz/`). Ils utilisent Qt Test et sont déclarés avec la fonction CMake `mangrove_add_test`, qui fait le `add_test` :

```cmake
# logiciel/tests/core/CMakeLists.txt
mangrove_add_test(tst_plan LIBS Mangrove::Core)
```

Le guide complet (écrire, nommer, lancer un test, labels `ctest -L core`…) est dans [logiciel/Documentation/guides/TESTING.md](../logiciel/Documentation/guides/TESTING.md).

Un exécutable de test ne peut pas linker contre l'exécutable `Mangrove` : le code testé doit être dans une bibliothèque (`mangrove_core`, `mangrove_app`). Un exécutable Qt Test renvoie le nombre de tests en échec, il fonctionne donc directement avec `ctest`.

### Règles pour qu'un test passe en CI

- **Pas d'écran** : la CI n'a pas d'affichage. Qt y est lancé en mode `offscreen` (`QT_QPA_PLATFORM=offscreen`). Un test ne doit pas attendre d'interaction utilisateur.
- **Pas de réseau** ni de service externe.
- **Déterministe** : même résultat à chaque lancement (pas de dépendance à l'heure, à l'ordre des tests ou à un aléatoire non fixé).
- **Fichiers** : ne pas utiliser de chemins absolus de votre machine. Écrire les fichiers temporaires avec `QTemporaryDir` ou dans le dossier courant.
- **Rapide** : quelques secondes par test au maximum. Le job entier est coupé après 30 minutes.
- **Messages d'erreur clairs** sur `stderr` : c'est ce qui s'affiche dans les logs quand le test échoue.

## API (JavaScript)

La pipeline de l'API n'est pas encore en place. Elle suivra le même principe : les tests seront lancés avec `npm test`, et le rapport sera publié dans le Summary du run. Cette section sera complétée avec le framework retenu.

## Dépannage

**Le test passe en local mais échoue en CI**
Vérifier les règles ci-dessus (écran, réseau, chemins absolus). Pour reproduire l'absence d'écran en local : `QT_QPA_PLATFORM=offscreen ctest --test-dir build --output-on-failure`.

**Mon test n'apparaît pas dans le Summary**
Il manque l'appel à `mangrove_add_test` dans le `CMakeLists.txt` du dossier de test. `ctest --test-dir build -N` liste les tests que ctest connaît.

**clang-tidy échoue en local avec `compile_commands.json` introuvable**
Le projet n'a pas été configuré dans `build/` : relancer la commande `cmake -S . -B build ...` ci-dessus.
