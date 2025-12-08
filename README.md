# Projet C - Manipulation de grands entiers en binaire (BigBinary)

## Objectif du projet
Le but de ce projet est de créer, en langage C, une bibliothèque permettant de manipuler des **grands entiers binaires** (appelés ici *BigBinary*).  
Un nombre est représenté sous forme de tableau dynamique de bits (`0` et `1`).


## Phase 1 :
Représentation d’un grand entier en binaire.
Fonctions de base : création, affichage, libération mémoire, comparaisons, addition et soustraction.

## Phase 2 :
Multiplication, division, modulo.
Algorithme du PGCD (version binaire).
Exponentiation modulaire rapide.

## Phase 3 (bonus) :
Utiliser la bibliothèque pour un mini-chiffrement RSA pédagogique (pas sécurisé, mais pour comprendre le principe).


## Organisation des fichiers

| Fichier | Rôle |
|----------|------|
| **bigbinary.h** | Déclaration de la structure `BigBinary` et des fonctions publiques. |
| **bigbinary.c** | Implémentation des fonctions de manipulation. |
| **main.c** | Programme principal pour tester les fonctions. |
| **README.md** | Description du projet et suivi de l’avancement. |

---

**État actuel :**

### **Phase 1 (terminée )**
**Objectif :** mise en place des bases de la structure et des opérations simples.

**Fonctionnalités réalisées :**
- Structure `BigBinary` (tableau dynamique de bits, taille, signe).
- Création :
    - depuis une chaîne binaire (`creerBigBinaryDepuisChaine`)
    - depuis un entier (`creerBigBinaryDepuisEntier`)
- Normalisation (suppression des zéros de tête).
- Affichage (`afficheBigBinary`)
- Libération mémoire (`libereBigBinary`)
- Comparaisons :
    - `egalBigBinary`
    - `inferieurBigBinary`
- Opérations arithmétiques :
    - `additionBigBinary`
    - `soustractionBigBinary` (si `A ≥ B`)

### **Phase 2 **
**Objectif :** ajouter les opérations arithmétiques avancées.

**Fonctionnalités prévues :**
- Multiplication (`multiplicationBigBinary`) 
- Division (`divisionBigBinary`)
- Modulo (`moduloBigBinary`)
- Algorithme du PGCD (version binaire)
- Exponentiation modulaire rapide



