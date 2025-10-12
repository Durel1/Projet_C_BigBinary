#ifndef BIGBINARY_H
#define BIGBINARY_H

// Définition de la structure BigBinary
typedef struct {
    int *Tdigits;   // Tableau dynamique contenant les bits
    // Convention : Tdigits[0] = MSB (bit de gauche)
    // Tdigits[Taille-1] = LSB (bit de droite)
    int Taille;     // Nombre de bits stockés
    int Signe;      // +1 si positif (Phase 1 : on ne gère que les positifs)
} BigBinary;

// Crée un BigBinary vide de taille donnée (bits = 0)
BigBinary initBigBinary(int taille, int signe);

// Crée un BigBinary à partir d'une chaîne binaire (ex: "1011")
BigBinary creerBigBinaryDepuisChaine(const char *s);

// Crée un BigBinary à partir d'un entier positif (ex: 13 -> "1101")
BigBinary creerBigBinaryDepuisEntier(int n);

// Supprime les zéros inutiles en tête
void normaliseBigBinary(BigBinary *nb);

// Affiche le BigBinary sur la sortie standard
void afficheBigBinary(const BigBinary nb);

// Libère la mémoire occupée par un BigBinary
void libereBigBinary(BigBinary *nb);

// Fonctions de comparaison

// Retourne 1 si A == B, sinon 0
int egalBigBinary(const BigBinary *A, const BigBinary *B);

// Retourne 1 si A < B, sinon 0
int inferieurBigBinary(const BigBinary *A, const BigBinary *B);

// Opérations arithmétiques de base

// Calcule A + B (retourne un nouveau BigBinary)
BigBinary additionBigBinary(const BigBinary *A, const BigBinary *B);

// Calcule A - B (retourne un nouveau BigBinary, suppose A >= B)
BigBinary soustractionBigBinary(const BigBinary *A, const BigBinary *B);

// Fonctions utilitaires pour la saisie
int estChaineBinaireValide(const char *s);
int estChaineDecimaleValide(const char *s);
BigBinary saisirBigBinaryAvecRetry();

#endif