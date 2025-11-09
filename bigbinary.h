#ifndef BIGBINARY_H
#define BIGBINARY_H

// Définition de la structure BigBinary
typedef struct {
    int *Tdigits;
    // Convention : Tdigits[0] = MSB (bit de gauche)
    // Tdigits[Taille-1] = LSB (bit de droite)
    int Taille;
    int Signe;
} BigBinary;

// === PHASE 1 - Fonctions de base ===

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
int egalBigBinary(const BigBinary *A, const BigBinary *B);
int inferieurBigBinary(const BigBinary *A, const BigBinary *B);

// Opérations arithmétiques de base
BigBinary additionBigBinary(const BigBinary *A, const BigBinary *B);  // A et B positifs
BigBinary soustractionBigBinary(const BigBinary *A, const BigBinary *B);  // A et B positifs, A >= B

// Fonctions utilitaires pour la saisie
int estChaineBinaireValide(const char *s);
int estChaineDecimaleValide(const char *s);
BigBinary saisirBigBinaryAvecRetry();

// === PHASE 2 - Fonctions avancées ===

// Fonctions utilitaires pour les opérations binaires
void decalageGauche(BigBinary *nb);    // Multiplication par 2 (ajoute un 0 à droite)
void decalageDroite(BigBinary *nb);    // Division par 2 (supprime le LSB)
int estPair(const BigBinary *nb);      // Vérifie si le nombre est pair (LSB = 0)

// Multiplication Égyptienne (algorithme de duplication)
BigBinary multiplicationEgyptienne(const BigBinary *A, const BigBinary *B);

// Algorithme binaire d'Euclide pour le PGCD
BigBinary pgcdBigBinary(const BigBinary *A, const BigBinary *B);

// Calcul du modulo sans division (méthode par soustraction de multiples)
BigBinary moduloBigBinary(const BigBinary *A, const BigBinary *B);

// Exponentiation modulaire rapide (exposant = unsigned int)
BigBinary expModBigBinary(const BigBinary *A, unsigned int e, const BigBinary *N);

// Fonction pour convertir BigBinary en entier décimal
unsigned long long bigBinaryVersDecimal(const BigBinary *nb);

#endif