# Mangrove

Éditeur de plans d'événements : gestion des exposants/stands et édition du plan via un canvas visuel.

## Structure du dépôt

| Dossier | Contenu |
|---------|---------|
| `logiciel/` | Application desktop Qt 6 / QML / C++20 (architecture et règles : [logiciel/README.md](logiciel/README.md)) |
| `docs/` | Documentation transverse (CI : [docs/CI.md](docs/CI.md)) |
| `.github/workflows/` | Pipeline GitHub Actions |

Le `CMakeLists.txt` et le `CMakePresets.json` à la racine ne font qu'inclure `logiciel/` : on configure, compile et teste **depuis la racine**, dans `build/`. C'est ce dossier que lisent la CI, le hook clang-tidy et clangd.

## Compiler et lancer

Prérequis : CMake ≥ 3.25, Ninja, un compilateur C++20 (GCC ≥ 13, Clang ≥ 17, MSVC 2022), Qt ≥ 6.8 (Core, Gui, Qml, Quick, QuickControls2, Test, LinguistTools).

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
QT_QPA_PLATFORM=offscreen ctest --test-dir build --output-on-failure
./build/logiciel/Mangrove
```

Sans Qt installé, les presets `linux-debug`, `linux-release` et `windows-*` passent par vcpkg, qui compile Qt (long la première fois) :

```sh
cmake --list-presets
cmake --preset linux-release && cmake --build --preset linux-release
```

> Les presets compilent dans `build/<preset>/`. Pour clang-tidy et clangd, qui lisent `build/compile_commands.json`, utilisez la commande `cmake -S . -B build` ci-dessus.

## Mise en place de l'environnement

Le dépôt utilise [pre-commit](https://pre-commit.com) pour vérifier automatiquement le code et les messages de commit avant chaque commit.

### 1. Installer pre-commit

pre-commit nécessite Python 3.9 ou plus récent.

```sh
pipx install pre-commit
```

Sans pipx :

```sh
python3 -m pip install --user pre-commit
```

> Utilisez `python3 -m pip` plutôt que `pip` : selon la machine, `pip` peut pointer vers une ancienne version de Python (Python 2 notamment) et installer une version obsolète de pre-commit.

Vérifiez la version installée (elle doit être 3.x ou plus récente) :

```sh
pre-commit --version
```

### 2. Installer les hooks dans le dépôt

À faire **une fois après chaque clone** :

```sh
pre-commit install --install-hooks
```

La commande doit afficher :

```
pre-commit installed at .git/hooks/pre-commit
pre-commit installed at .git/hooks/commit-msg
```

### Vérifications effectuées

| Hook | Étape | Rôle |
|------|-------|------|
| `clang-format` | pre-commit | Formate le code C++ selon le `.clang-format` du dépôt |
| `clang-tidy` | pre-commit | Analyse statique du code C++ |
| `conventional-pre-commit` | commit-msg | Vérifie que le message de commit respecte la convention ci-dessous |

Pour lancer les vérifications sur tout le dépôt sans commiter :

```sh
pre-commit run --all-files
```

### Formater et analyser le code à la main

Le format suit le `.clang-format` le plus proche du fichier : `logiciel/.clang-format`, surchargé par `logiciel/src/app/.clang-format` pour la couche app. clang-tidy suit le `.clang-tidy` de la racine et a besoin de `build/compile_commands.json` : **configurez le projet dans `build/` avant**.

```sh
# Formater tout le code (modifie les fichiers)
pre-commit run clang-format --all-files

# Analyse statique (après cmake -S . -B build)
pre-commit run clang-tidy --all-files

# Seulement les fichiers modifiés et indexés (ce que fait le hook au commit)
pre-commit run
```

Les hooks utilisent clang-format/clang-tidy **21**, téléchargés par pre-commit : la version installée sur votre machine peut formater légèrement différemment, c'est celle du hook qui fait foi en CI.

## CI et tests

La CI (GitHub Actions) vérifie le formatage, compile le projet et lance les tests à chaque push. Pour savoir comment écrire des tests compatibles et lire les résultats : [docs/CI.md](docs/CI.md).

## Convention de commit

Les messages de commit suivent [Conventional Commits](https://www.conventionalcommits.org/fr/) :

```
<type>(<scope>): <description>
```

- **type** : la nature du changement (voir tableau)
- **scope** (optionnel) : le module ou le fichier concerné, par exemple `Stand`, `canvas`, `mobile`
- **description** : courte, à l'impératif, sans majuscule ni point final

| Type | Usage |
|------|-------|
| `feat` | Nouvelle fonctionnalité |
| `fix` | Correction de bug |
| `docs` | Documentation uniquement |
| `style` | Formatage, sans changement de comportement |
| `refactor` | Restructuration du code, sans nouvelle fonctionnalité ni correction |
| `perf` | Amélioration des performances |
| `test` | Ajout ou modification de tests |
| `build` | Système de build, dépendances |
| `ci` | Configuration de la CI |
| `chore` | Maintenance diverse |

Exemples :

```
feat(Stand): add constraint button
fix(canvas): improve line handling
docs(README): add commit convention
```

Un changement cassant la compatibilité s'indique avec `!` : `feat(api)!: remove legacy endpoint`.

## Dépannage

**`Unexpected key(s) present at root: default_install_hook_types`**
Votre version de pre-commit est trop ancienne. Désinstallez-la (`pip uninstall pre-commit`, ou `python2 -m pip uninstall pre-commit` si elle a été installée avec Python 2) puis réinstallez-la avec `pipx` comme indiqué plus haut.

**Le hook `commit-msg` ne se déclenche pas**
Relancez `pre-commit install --install-hooks` et vérifiez que `.git/hooks/commit-msg` existe.

**Contourner les hooks ponctuellement**
`git commit --no-verify` désactive les hooks pour un commit. À éviter : les mêmes vérifications doivent passer en CI.
