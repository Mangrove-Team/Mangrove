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
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build --parallel
pre-commit run --all-files
ctest --test-dir build --output-on-failure
```

> Le dossier de build doit s'appeler **`build/`** : le hook clang-tidy lit `build/compile_commands.json`. Sous CLion, changer le dossier dans *Settings → Build, Execution, Deployment → CMake → Build directory*.

## Écrire un test C++ compatible

### Le principe

La CI lance **`ctest`**, qui exécute tous les tests déclarés dans CMake. Un test est simplement un **exécutable** :

- code de sortie **0** → test réussi ;
- tout autre code de sortie (ou un crash) → test échoué.

Un test qui n'est pas déclaré avec `add_test` n'est **pas lancé** par la CI, même s'il compile.

### Où placer les tests

```
tests/
├── CMakeLists.txt        ← déclaration des tests
├── sanity_test.cpp       ← exemple minimal
└── <module>_test.cpp     ← un fichier par module testé
```

Nommage : `<module>_test.cpp`, et le même nom pour l'exécutable et le test (ex. `stand_test`).

### Déclarer un test

Dans `tests/CMakeLists.txt` :

```cmake
add_executable(stand_test stand_test.cpp)
target_link_libraries(stand_test PRIVATE mangrove_core)
add_test(NAME stand_test COMMAND stand_test)
```

### Tester le code de l'application

Un exécutable de test ne peut pas linker contre l'exécutable `Mangrove`. Le code métier (Plan, Stand, Constraint…) doit donc être compilé dans une **bibliothèque** que l'application et les tests utilisent tous les deux :

```cmake
add_library(mangrove_core STATIC
        src/Plan/Plan.cpp
        src/Stand/Stand.cpp
        # ...
)
target_include_directories(mangrove_core PUBLIC src/Plan src/Stand ...)
target_link_libraries(mangrove_core PUBLIC Qt6::Core)

add_executable(Mangrove src/main.cpp ...)
target_link_libraries(Mangrove PRIVATE mangrove_core Qt6::Quick Qt6::Svg)
```

### Exemple minimal (sans framework)

Voir `tests/sanity_test.cpp` :

```cpp
#include <cstdlib>
#include <iostream>

int main () {
    if (2 + 2 != 4) {
        std::cerr << "sanity_test: 2 + 2 != 4\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
```

### Avec Qt Test

Qt Test est déjà installé en CI avec Qt, sans dépendance supplémentaire. Un exécutable Qt Test renvoie le nombre de tests en échec, donc il fonctionne directement avec `ctest` :

```cmake
find_package(Qt6 REQUIRED COMPONENTS Test)

add_executable(stand_test stand_test.cpp)
target_link_libraries(stand_test PRIVATE mangrove_core Qt6::Test)
add_test(NAME stand_test COMMAND stand_test)
```

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
Il manque le `add_test` dans `tests/CMakeLists.txt`. `ctest --test-dir build -N` liste les tests que ctest connaît.

**clang-tidy échoue en local avec `compile_commands.json` introuvable**
Le projet n'a pas été configuré dans `build/` : relancer la commande `cmake -S . -B build ...` ci-dessus.
