# Mangrove

Éditeur de plans d'événements : gestion des exposants/stands et édition du plan via un canvas visuel.

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
