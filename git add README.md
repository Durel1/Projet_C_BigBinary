# Projet C - Manipulation de grands entiers en binaire (BigBinary)

Objectif du projet
Le but de ce projet est de créer en langage C une bibliothèque pour manipuler des grands entiers binaires (qu’on appelle ici `BigBinary`).
Un nombre est représenté comme un tableau de bits (0 et 1).  
On implémente progressivement des opérations de plus en plus complexes :

1. Phase 1 (obligatoire) :
    - Représentation d’un grand entier en binaire.
    - Fonctions de base : création depuis , affichage, libération mémoire, comparaisons, addition et soustraction.

2. Phase 2 (avancé) :
    - Multiplication, division, modulo.
    - Algorithme du PGCD (version binaire).
    - Exponentiation modulaire rapide.

3. Phase 3 (bonus) :
    - Utiliser la bibliothèque pour un mini-chiffrement RSA pédagogique (pas sécurisé, mais pour comprendre le principe).


Organisation des fichiers
- `bigbinary.h` : le header  (déclare la structure `BigBinary` et les fonctions publiques).
- `bigbinary.c` : l’implémentation (code des fonctions de manipulation).
- `main.c` : le programme principal pour tester les fonctions.
- `README.md` : ce fichier, qui explique le projet et son avancement.

Phase 1  
Fonctionnalités disponibles :
- Structure " BigBinary " : tableau dynamique de bits, taille, signe.
- Création :
    - Depuis une chaîne binaire (`creerBigBinaryDepuisChaine`)
    - Depuis un entier (`creerBigBinaryDepuisEntier`)
- Normalisation : suppression des zéros de tête.
- Affichage (`afficheBigBinary`).
- Libération mémoire (`libereBigBinary`).
- Comparaisons : `egalBigBinary`, `inferieurBigBinary`.
- Opérations arithmétiques : `additionBigBinary`, `soustractionBigBinary` (si A ≥ B).``

  Fonctionnement du programme
1. L'utilisateur saisit deux entiers au clavier.
2. Ils sont automatiquement convertis en binaire.
3. Le programme affiche leur représentation binaire.
4. Il effectue :
    - les comparaisons (`==`, `<`),
    - l'addition,
    - et la soustraction si A ≥ B.

    