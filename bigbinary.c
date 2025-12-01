#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bigbinary.h"

// PHASE 1 - FONCTIONS DE BASE

// Initialise un BigBinary avec une taille donnée, tous les bits sont initialisés à 0
BigBinary initBigBinary(int taille, int signe) {
    BigBinary nb;
    nb.Taille = taille;         // On stocke la taille
    nb.Signe = signe;           // Pour l'instant, toujours +1 (positif)
    nb.Tdigits = (int *)calloc(taille, sizeof(int)); // On réserve un tableau rempli de 0
    return nb;
}

// Saisit un BigBinary avec réessai infini en cas d'erreur
BigBinary saisirBigBinaryAvecRetry() {
    char buffer[256];
    while (1) {  // Boucle infinie jusqu'à ce qu'une entrée valide soit fournie
        printf("> ");
        if (scanf("%255s", buffer) != 1) {
            // Erreur de lecture
            printf("Erreur de lecture. Veuillez réessayer.");
            // Vider le buffer d'entrée
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        // Vérifier si c'est une chaîne binaire valide
        if (estChaineBinaireValide(buffer)) {
            return creerBigBinaryDepuisChaine(buffer);
        }
        // Vérifier si c'est une chaîne décimale valide
        if (estChaineDecimaleValide(buffer)) {
            int valeur = atoi(buffer);
            return creerBigBinaryDepuisEntier(valeur);
        }
        // Si on arrive ici, l'entrée est invalide
        printf("Entree invalide. Veuillez entrer un nombre binaire (ex: 1011) ou decimal (ex: 42) :\n");
        // Vider le buffer d'entrée
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
}

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

// Crée un BigBinary depuis une chaîne binaire
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
void normaliseBigBinary(BigBinary *nb) {
    int i = 0;
    // On avance tant qu'il y a des 0 à gauche, sauf si on arrive au dernier bit
    while (i < nb->Taille - 1 && nb->Tdigits[i] == 0) {
        i++;
    }
    // Si on a trouvé des zéros à enlever
    if (i > 0) {
        int newSize = nb->Taille - i;         // Nouvelle taille sans les zéros
        int *newTab = (int *)malloc(newSize * sizeof(int)); // Alloue nouveau tableau
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

// Addition binaire A + B (A et B positifs)
// On part du dernier bit (LSB) et on remonte vers la gauche
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

// Soustraction binaire A - B (A et B positifs, A >= B)
BigBinary soustractionBigBinary(const BigBinary *A, const BigBinary *B) {
    // Vérification que A >= B
    if (inferieurBigBinary(A, B)) {
        printf("Operation impossible : A < B\n");
        // Retourne un BigBinary nul
        return creerBigBinaryDepuisEntier(0);
    }

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

    normaliseBigBinary(&R);
    return R;
}

// Convertit un BigBinary en entier décimal
unsigned long long bigBinaryVersDecimal(const BigBinary *nb) {
    unsigned long long resultat = 0;
    unsigned long long puissance = 1;

    // On part de la fin (LSB) vers le début (MSB)
    for (int i = nb->Taille - 1; i >= 0; i--) {
        if (nb->Tdigits[i] == 1) {
            resultat += puissance;
        }
        puissance *= 2;
    }
    return resultat;
}


// PHASE 2 - FONCTIONS AVANCÉES

// Effectue un décalage à gauche (multiplication par 2)
void decalageGauche(BigBinary *nb) {
    if (nb->Taille == 1 && nb->Tdigits[0] == 0) {
        return; // 0 décalé à gauche reste 0
    }

    int newSize = nb->Taille + 1;
    int *newTab = (int *)malloc(newSize * sizeof(int));

    // Recopie des bits existants
    for (int i = 0; i < nb->Taille; i++) {
        newTab[i] = nb->Tdigits[i];
    }
    // Ajoute un 0 à la fin (LSB)
    newTab[newSize - 1] = 0;

    free(nb->Tdigits);
    nb->Tdigits = newTab;
    nb->Taille = newSize;
}

// Effectue un décalage à droite (division entière par 2)
void decalageDroite(BigBinary *nb) {
    if (nb->Taille <= 1) {
        // Si taille 1 ou 0, le résultat est 0
        if (nb->Taille == 1) {
            nb->Tdigits[0] = 0;
        }
        return;
    }

    int newSize = nb->Taille - 1;
    int *newTab = (int *)malloc(newSize * sizeof(int));

    // Recopie sans le dernier bit (LSB)
    for (int i = 0; i < newSize; i++) {
        newTab[i] = nb->Tdigits[i];
    }

    free(nb->Tdigits);
    nb->Tdigits = newTab;
    nb->Taille = newSize;

    normaliseBigBinary(nb);
}

// Vérifie si un BigBinary est pair (LSB = 0)
int estPair(const BigBinary *nb) {
    if (nb->Taille == 0) return 1; // 0 est considéré comme pair
    return (nb->Tdigits[nb->Taille - 1] == 0); // Vérifie le bit de poids faible
}

// Multiplication Égyptienne (algorithme de duplication)
BigBinary multiplicationEgyptienne(const BigBinary *A, const BigBinary *B) {
    BigBinary resultat = creerBigBinaryDepuisEntier(0); // Résultat initialisé à 0
    BigBinary multiplicande;  // Copie de A (qu'on va décaler)
    BigBinary multiplicateur; // Copie de B (qu'on va réduire)

    // Créer des copies pour travailler
    multiplicande.Taille = A->Taille;
    multiplicande.Signe = A->Signe;
    multiplicande.Tdigits = (int *)malloc(multiplicande.Taille * sizeof(int));
    memcpy(multiplicande.Tdigits, A->Tdigits, multiplicande.Taille * sizeof(int));

    multiplicateur.Taille = B->Taille;
    multiplicateur.Signe = B->Signe;
    multiplicateur.Tdigits = (int *)malloc(multiplicateur.Taille * sizeof(int));
    memcpy(multiplicateur.Tdigits, B->Tdigits, multiplicateur.Taille * sizeof(int));

    // Tant que le multiplicateur n'est pas nul
    while (multiplicateur.Taille > 1 || multiplicateur.Tdigits[0] != 0) {
        // Si le bit de poids faible du multiplicateur est 1, on ajoute le multiplicande
        if (!estPair(&multiplicateur)) {
            BigBinary temp = additionBigBinary(&resultat, &multiplicande);
            libereBigBinary(&resultat);
            resultat = temp;
        }

        // Décalage à gauche du multiplicande (×2)
        decalageGauche(&multiplicande);

        // Décalage à droite du multiplicateur (÷2)
        decalageDroite(&multiplicateur);
    }

    // Libération mémoire des copies
    libereBigBinary(&multiplicande);
    libereBigBinary(&multiplicateur);

    return resultat;
}

// Algorithme binaire d'Euclide pour calculer le PGCD
BigBinary pgcdBigBinary(const BigBinary *A, const BigBinary *B) {
    // Cas de base : si B = 0, PGCD = A
    if (B->Taille == 1 && B->Tdigits[0] == 0) {
        BigBinary resultat;
        resultat.Taille = A->Taille;
        resultat.Signe = A->Signe;
        resultat.Tdigits = (int *)malloc(resultat.Taille * sizeof(int));
        memcpy(resultat.Tdigits, A->Tdigits, resultat.Taille * sizeof(int));
        return resultat;
    }

    // Cas de base : si A = 0, PGCD = B
    if (A->Taille == 1 && A->Tdigits[0] == 0) {
        BigBinary resultat;
        resultat.Taille = B->Taille;
        resultat.Signe = B->Signe;
        resultat.Tdigits = (int *)malloc(resultat.Taille * sizeof(int));
        memcpy(resultat.Tdigits, B->Tdigits, resultat.Taille * sizeof(int));
        return resultat;
    }

    // Faire des copies pour travailler sans modifier les originaux
    BigBinary a, b;
    a.Taille = A->Taille;
    a.Signe = A->Signe;
    a.Tdigits = (int *)malloc(a.Taille * sizeof(int));
    memcpy(a.Tdigits, A->Tdigits, a.Taille * sizeof(int));

    b.Taille = B->Taille;
    b.Signe = B->Signe;
    b.Tdigits = (int *)malloc(b.Taille * sizeof(int));
    memcpy(b.Tdigits, B->Tdigits, b.Taille * sizeof(int));

    int facteur = 0;

    // Éliminer les facteurs 2 communs
    while (estPair(&a) && estPair(&b)) {
        decalageDroite(&a);
        decalageDroite(&b);
        facteur++;
    }

    // Algorithme principal
    while (a.Taille > 1 || a.Tdigits[0] != 0) {
        // Éliminer les facteurs 2 de a
        while (estPair(&a)) {
            decalageDroite(&a);
        }
        // Éliminer les facteurs 2 de b
        while (estPair(&b)) {
            decalageDroite(&b);
        }

        // Soustraire le plus petit du plus grand
        if (!inferieurBigBinary(&a, &b)) {
            BigBinary temp = soustractionBigBinary(&a, &b);
            libereBigBinary(&a);
            a = temp;
        } else {
            BigBinary temp = soustractionBigBinary(&b, &a);
            libereBigBinary(&b);
            b = temp;
        }
    }

    // Le PGCD est b multiplié par les facteurs 2 communs
    BigBinary resultat = b;
    for (int i = 0; i < facteur; i++) {
        decalageGauche(&resultat);
    }

    libereBigBinary(&a);
    return resultat;
}

// Utilisation de la méthode par soustractions successives
BigBinary moduloBigBinary(const BigBinary *A, const BigBinary *B) {
    // Cas particuliers
    if (B->Taille == 1 && B->Tdigits[0] == 0) {
        printf("Erreur : division par zero\n");
        return creerBigBinaryDepuisEntier(0);
    }

    if (inferieurBigBinary(A, B)) {
        // Si A < B, alors A mod B = A
        BigBinary resultat;
        resultat.Taille = A->Taille;
        resultat.Signe = A->Signe;
        resultat.Tdigits = (int *)malloc(resultat.Taille * sizeof(int));
        memcpy(resultat.Tdigits, A->Tdigits, resultat.Taille * sizeof(int));
        return resultat;
    }

    if (egalBigBinary(A, B)) {
        // Si A = B, alors A mod B = 0
        return creerBigBinaryDepuisEntier(0);
    }

    // Faire une copie de A pour travailler
    BigBinary a;
    a.Taille = A->Taille;
    a.Signe = A->Signe;
    a.Tdigits = (int *)malloc(a.Taille * sizeof(int));
    memcpy(a.Tdigits, A->Tdigits, a.Taille * sizeof(int));

    // Méthode simple : soustraire B jusqu'à ce que a < B
    BigBinary b_copy;
    b_copy.Taille = B->Taille;
    b_copy.Signe = B->Signe;
    b_copy.Tdigits = (int *)malloc(b_copy.Taille * sizeof(int));
    memcpy(b_copy.Tdigits, B->Tdigits, b_copy.Taille * sizeof(int));

    // Tant que a >= B, soustraire B
    while (!inferieurBigBinary(&a, &b_copy)) {
        BigBinary temp = soustractionBigBinary(&a, &b_copy);
        libereBigBinary(&a);
        a = temp;
    }
    libereBigBinary(&b_copy);
    return a;
}

// Exponentiation modulaire rapide A^e mod N
BigBinary expModBigBinary(const BigBinary *A, unsigned int e, const BigBinary *N) {
    // Cas de base
    if (e == 0) {
        return creerBigBinaryDepuisEntier(1); // A^0 = 1
    }

    // Réduire A modulo N d'abord pour s'assurer que base < N
    BigBinary base = moduloBigBinary(A, N);

    BigBinary resultat = creerBigBinaryDepuisEntier(1);

    // Algorithme d'exponentiation binaire
    while (e > 0) {
        // Si e est impair, multiplier le résultat par la base
        if (e % 2 == 1) {
            BigBinary temp = multiplicationEgyptienne(&resultat, &base);
            BigBinary temp_mod = moduloBigBinary(&temp, N);
            libereBigBinary(&resultat);
            libereBigBinary(&temp);
            resultat = temp_mod;
        }
        // Mettre la base au carré
        if (e > 1) { // Éviter un calcul inutile à la dernière itération
            BigBinary temp_carre = multiplicationEgyptienne(&base, &base);
            BigBinary base_mod = moduloBigBinary(&temp_carre, N);
            libereBigBinary(&base);
            libereBigBinary(&temp_carre);
            base = base_mod;
        }
        // Diviser e par 2
        e /= 2;
    }
    libereBigBinary(&base);
    return resultat;
}
