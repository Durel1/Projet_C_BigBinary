#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bigbinary.h"

// Initialise un BigBinary avec une taille donnée, tous les bits sont initialisés à 0
BigBinary initBigBinary(int taille, int signe) {
    BigBinary nb;
    nb.Taille = taille;
    nb.Signe = signe;
    nb.Tdigits = (int *)calloc(taille, sizeof(int)); // On réserve un tableau rempli de 0
    return nb;
}

// Saisit un BigBinary avec réessai infini en cas d'erreur
BigBinary saisirBigBinaryAvecRetry() {
    char buffer[256];
    while (1) {
        printf("> ");
        if (scanf("%255s", buffer) != 1) {
            printf("Erreur de lecture. Veuillez réessayer.");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }
        int signe = 1; // Le signe par défaut est positif
        char* num_str = buffer; // Pointeur vers le début de la partie nombre

        if (buffer[0] == '-') {
            signe = -1;
            num_str++; // Avancer le pointeur après le '-'
        } else if (buffer[0] == '+') {
            num_str++; // Avancer le pointeur après le '+'
        }
        BigBinary result;

        // Vérifier si c'est une chaîne binaire valide
        if (estChaineBinaireValide(num_str)) {
            result = creerBigBinaryDepuisChaine(num_str);
            result.Signe = signe;
            //printf("DEBUG: Conversion reussie. Taille = %d, Signe = %d\n", result.Taille, result.Signe);
            return result;
        }
        // Vérifier si c'est une chaîne décimale valide
        if (estChaineDecimaleValide(num_str)) {
            int valeur_abs = atoi(num_str);
            result = creerBigBinaryDepuisEntier(valeur_abs);
            result.Signe = signe;
            //printf("DEBUG: Conversion reussie. Taille = %d, Signe = %d\n", result.Taille, result.Signe);
            return result;
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
    // Vérification et affichage du signe
    if (nb.Signe == -1) {
        printf("-");
    }
    // Cas spécial pour le zéro (si Taille=0 ou Signe=0)
    if (nb.Signe == 0 || (nb.Taille == 1 && nb.Tdigits[0] == 0)) {
        printf("0");
        printf("\n");
        return;
    }
    // Affichage des bits (du MSB au LSB)
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
    if (A->Taille != B->Taille) return 0;
    for (int i = 0; i < A->Taille; i++) {
        if (A->Tdigits[i] != B->Tdigits[i]) return 0;
    }
    return 1;
}

// Retourne 1 si A < B, sinon 0
int inferieurBigBinary(const BigBinary *A, const BigBinary *B) {
    if (A->Taille < B->Taille) return 1;      // Moins de bits donc forcément plus petit
    if (A->Taille > B->Taille) return 0;      // Plus de bits donc forcément plus grand
    // Sinon, on compare bit par bit
    for (int i = 0; i < A->Taille; i++) {
        if (A->Tdigits[i] < B->Tdigits[i]) return 1;
        if (A->Tdigits[i] > B->Tdigits[i]) return 0;
    }
    return 0; // sont égaux
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

// Crée une copie en profondeur d'un BigBinary
BigBinary copierBigBinary(const BigBinary *nb) {
    BigBinary copie;
    copie.Taille = nb->Taille;
    copie.Signe = nb->Signe;
    copie.Tdigits = (int *)malloc(copie.Taille * sizeof(int));
    memcpy(copie.Tdigits, nb->Tdigits, copie.Taille * sizeof(int));
    return copie;
}

// Utilisation de la méthode par soustraction de multiples (optimisée)
BigBinary moduloBigBinary(const BigBinary *A, const BigBinary *B) {
    // Cas particuliers
    if (B->Taille == 1 && B->Tdigits[0] == 0) {
        printf("Erreur : division par zero\n");
        return creerBigBinaryDepuisEntier(0);
    }
    if (inferieurBigBinary(A, B)) {
        // Si A < B, alors A mod B = A
        return copierBigBinary(A); // Copie pour ne pas modifier l'original
    }
    if (egalBigBinary(A, B)) {
        // Si A = B, alors A mod B = 0
        return creerBigBinaryDepuisEntier(0);
    }

    // --- PRÉPARATION ---

    // Fonction d'utilité pour créer une copie d'un BigBinary (Nécessaire !)
    // (J'ai ajouté cette fonction pour la clarté et la sécurité mémoire)
    // BigBinary copierBigBinary(const BigBinary *nb) { ... }
    BigBinary dividende = copierBigBinary(A);
    BigBinary diviseur_B = copierBigBinary(B);
    BigBinary temp_B; // Pour stocker B décalé (2^k * B)

    // --- ALGORITHME DE RÉDUCTION PAR MULTIPLES DE 2 ---

    // 1. Décaler B (diviseur) jusqu'à ce qu'il ait presque la même taille que A (dividende)
    int decalage_k = dividende.Taille - diviseur_B.Taille;

    // Décalage initial
    temp_B = copierBigBinary(&diviseur_B);
    for (int i = 0; i < decalage_k; i++) {
        decalageGauche(&temp_B);
    }

    // Si B * 2^k est plus grand que A, on enlève un décalage
    if (inferieurBigBinary(&dividende, &temp_B)) {
        decalageDroite(&temp_B);
        decalage_k--;
    }

    // 2. Boucle de soustraction optimisée (de 2^k * B jusqu'à 2^0 * B)
    for (int k = decalage_k; k >= 0; k--) {
        // Vérifie si 2^k * B peut être soustrait de A
        if (!inferieurBigBinary(&dividende, &temp_B)) {
            // Soustraction : A = A - (2^k * B)
            BigBinary resultat_temp = soustractionBigBinary(&dividende, &temp_B);
            libereBigBinary(&dividende);
            dividende = resultat_temp;
        }

        // On divise 2^k * B par 2 pour passer à l'itération suivante (2^(k-1) * B)
        if (k > 0) {
            decalageDroite(&temp_B);
        }
    }

    // --- NETTOYAGE ET FIN ---
    libereBigBinary(&diviseur_B);
    libereBigBinary(&temp_B);

    // 'dividende' contient maintenant le reste (A mod B)
    return dividende;
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

// === PHASE 3 (BONUS) - Fonctions RSA simplifiées ===

// Chiffrement RSA: C = M^E mod N
// M: Message (BigBinary)
// E: Exposant public (unsigned int)
// N: Module RSA (BigBinary)
// Retourne C, le message chiffré
BigBinary BigBinary_RSA_encrypt(const BigBinary *M, unsigned int E, const BigBinary *N) {
    // La fonction expModBigBinary fait tout le travail : M^E mod N
    return expModBigBinary(M, E, N);
}

// Déchiffrement RSA: M = C^D mod N
// C: Message chiffré (BigBinary)
// D: Exposant privé (unsigned int)
// N: Module RSA (BigBinary)
// Retourne M, le message déchiffré
BigBinary BigBinary_RSA_decrypt(const BigBinary *C, unsigned int D, const BigBinary *N) {
    // La fonction expModBigBinary fait tout le travail : C^D mod N
    return expModBigBinary(C, D, N);
}