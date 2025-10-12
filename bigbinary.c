#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bigbinary.h"

// Vérifie si une chaîne est valide (contient seulement '0' et '1')
int estChaineBinaireValide(const char *s) {
    if (s == NULL || strlen(s) == 0) return 0;

    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] != '0' && s[i] != '1') {
            return 0;
        }
    }
    return 1;
}

// Vérifie si une chaîne est un entier décimal valide
int estChaineDecimaleValide(const char *s) {
    if (s == NULL || strlen(s) == 0) return 0;

    for (int i = 0; s[i] != '\0'; i++) {
        if (!isdigit(s[i])) {
            return 0;
        }
    }
    return 1;
}

// Saisit un BigBinary avec réessai infini en cas d'erreur
BigBinary saisirBigBinaryAvecRetry() {
    char buffer[256];

    while (1) {  // Boucle infinie jusqu'à ce qu'une entrée valide soit fournie
        printf("> ");

        if (scanf("%255s", buffer) != 1) {
            // Erreur de lecture
            printf("Erreur de lecture. Veuillez réessayer.\n");

            // Vider le buffer d'entrée
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        // Vérifier si c'est une chaîne binaire valide
        if (estChaineBinaireValide(buffer)) {
            printf("Interprete comme binaire: %s\n", buffer);
            return creerBigBinaryDepuisChaine(buffer);
        }

        // Vérifier si c'est une chaîne décimale valide
        if (estChaineDecimaleValide(buffer)) {
            int valeur = atoi(buffer);
            printf("Interprete comme decimal: %d\n", valeur);
            return creerBigBinaryDepuisEntier(valeur);
        }

        // Si on arrive ici, l'entrée est invalide
        printf(" Entree invalide. Veuillez entrer un nombre binaire (ex: 1011) ou decimal (ex: 42) :\n");

        // Vider le buffer d'entrée
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

// Le reste du code bigbinary.c reste identique...
// [Toutes les autres fonctions restent les mêmes]

// Initialise un BigBinary avec une taille donnée
// Tous les bits sont initialisés a 0
BigBinary initBigBinary(int taille, int signe) {
    BigBinary nb;
    nb.Taille = taille;         // On stocke la taille
    nb.Signe = signe;           // Pour l'instant, toujours +1 (positif)
    nb.Tdigits = (int *)calloc(taille, sizeof(int)); // On réserve un tableau rempli de 0
    return nb;
}

// Crée un BigBinary depuis une chaîne binaire
// Exemple : "1011" -> tableau [1,0,1,1]
BigBinary creerBigBinaryDepuisChaine(const char *s) {
    int len = strlen(s);                      // Taille de la chaîne
    BigBinary nb = initBigBinary(len, +1);    // On réserve un BigBinary de cette taille

    // On copie chaque caractère de la chaîne dans le tableau
    // s[0] devient Tdigits[0] (MSB)
    for (int i = 0; i < len; i++) {
        nb.Tdigits[i] = (s[i] == '1') ? 1 : 0;
    }

    // On supprime les éventuels zéros inutiles au début
    normaliseBigBinary(&nb);
    return nb;
}

// Crée un BigBinary à partir d'un entier positif (int)
// Exemple : 13 -> "1101"
BigBinary creerBigBinaryDepuisEntier(int n) {
    if (n == 0) return creerBigBinaryDepuisChaine("0");

    // Calcul du nombre de bits nécessaires
    int nbBits = 0, temp = n;
    while (temp > 0) {
        nbBits++;
        temp /= 2;
    }

    BigBinary nb = initBigBinary(nbBits, +1);

    // Remplissage du tableau de droite à gauche (LSB à la fin)
    int i = nbBits - 1;
    while (n > 0) {
        nb.Tdigits[i] = n % 2;  // on prend le reste de la division par 2 (bit LSB)
        n /= 2;                 // on divise par 2 pour passer au bit suivant
        i--;
    }

    return nb;
}

// Supprime les zéros inutiles au début du tableau
// Exemple : "000101" -> "101"
void normaliseBigBinary(BigBinary *nb) {
    int i = 0;
    // On avance tant qu'il y a des 0 à gauche, sauf si on arrive au dernier bit
    while (i < nb->Taille - 1 && nb->Tdigits[i] == 0) {
        i++;
    }

    // Si on a trouvé des zéros à enlever
    if (i > 0) {
        int newSize = nb->Taille - i;         // Nouvelle taille sans les zéros
        int *newTab = (int *)malloc(newSize * sizeof(int));
        for (int j = 0; j < newSize; j++) {
            newTab[j] = nb->Tdigits[j + i];   // On recopie sans les zéros
        }
        free(nb->Tdigits);                    // On libère l'ancien tableau
        nb->Tdigits = newTab;                 // On met à jour
        nb->Taille = newSize;
    }

    // Cas spécial : si après normalisation on a un tableau vide, on garde au moins un 0
    if (nb->Taille == 0) {
        nb->Taille = 1;
        nb->Tdigits = (int *)malloc(sizeof(int));
        nb->Tdigits[0] = 0;
    }
}

// Affiche un BigBinary
// Exemple : tableau [1,0,1,1] -> affiche "1011"
void afficheBigBinary(const BigBinary nb) {
    for (int i = 0; i < nb.Taille; i++) {
        printf("%d", nb.Tdigits[i]);
    }
    printf("\n");
}

// Libère la mémoire occupée par un BigBinary
void libereBigBinary(BigBinary *nb) {
    if (nb->Tdigits != NULL) {
        free(nb->Tdigits);
        nb->Tdigits = NULL;
    }
    nb->Taille = 0;
    nb->Signe = +1;
}

// Compare si deux BigBinary sont égaux
int egalBigBinary(const BigBinary *A, const BigBinary *B) {
    if (A->Taille != B->Taille) return 0;     // Différentes tailles -> faux
    for (int i = 0; i < A->Taille; i++) {
        if (A->Tdigits[i] != B->Tdigits[i]) return 0; // Un bit différent -> faux
    }
    return 1; // Tous les bits identiques
}

// Retourne 1 si A < B, sinon 0
int inferieurBigBinary(const BigBinary *A, const BigBinary *B) {
    if (A->Taille < B->Taille) return 1;      // Moins de bits -> forcément plus petit
    if (A->Taille > B->Taille) return 0;      // Plus de bits -> forcément plus grand

    // Sinon, on compare bit par bit
    for (int i = 0; i < A->Taille; i++) {
        if (A->Tdigits[i] < B->Tdigits[i]) return 1;
        if (A->Tdigits[i] > B->Tdigits[i]) return 0;
    }
    return 0; // Exactement égaux
}

// Addition binaire A + B
// On part du dernier bit (LSB) et on remonte vers la gauche
// Exemple : 1011 (11) + 0101 (5) = 10000 (16)
BigBinary additionBigBinary(const BigBinary *A, const BigBinary *B) {
    int lenA = A->Taille, lenB = B->Taille;
    int maxLen = (lenA > lenB) ? lenA : lenB;
    int carry = 0; // Retenue

    // Résultat peut avoir un bit de plus
    BigBinary R = initBigBinary(maxLen + 1, +1);

    // On commence depuis la fin (LSB)
    int iA = lenA - 1, iB = lenB - 1, iR = maxLen;

    while (iR >= 0) {
        int bitA = (iA >= 0) ? A->Tdigits[iA] : 0;
        int bitB = (iB >= 0) ? B->Tdigits[iB] : 0;
        int sum = bitA + bitB + carry;
        R.Tdigits[iR] = sum % 2;  // On garde le bit courant
        carry = sum / 2;          // Retenue (0 ou 1)
        iA--; iB--; iR--;
    }

    normaliseBigBinary(&R);
    return R;
}

// Soustraction binaire A - B (en supposant A >= B)
// Exemple : 10000 (16) - 0101 (5) = 1011 (11)
BigBinary soustractionBigBinary(const BigBinary *A, const BigBinary *B) {
    int lenA = A->Taille, lenB = B->Taille;
    BigBinary R = initBigBinary(lenA, +1);

    int borrow = 0; // Retenue négative
    int iA = lenA - 1, iB = lenB - 1, iR = lenA - 1;

    while (iR >= 0) {
        int bitA = (iA >= 0) ? A->Tdigits[iA] : 0;
        int bitB = (iB >= 0) ? B->Tdigits[iB] : 0;
        int diff = bitA - bitB - borrow;
        if (diff < 0) {
            diff += 2;   // On ajoute 2 pour compenser (on a emprunté)
            borrow = 1;
        } else {
            borrow = 0;
        }
        R.Tdigits[iR] = diff;
        iA--; iB--; iR--;
    }

    // Vérification que A >= B
    if (borrow != 0) {
        printf("Attention : Resultat negatif detecte (A < B)\n");
        // On retourne 0 pour éviter les résultats incorrects
        libereBigBinary(&R);
        return creerBigBinaryDepuisEntier(0);
    }

    normaliseBigBinary(&R);
    return R;
}