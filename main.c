#include <stdio.h>
#include "bigbinary.h"
#include "bigbinary.c"

int main() {
    char continuer = 'o';
    printf("=== BIBLIOTHEQUE BIG BINARY === \n\n");
    do {
        printf("Entrez le premier nombre (A) :\n");
        BigBinary A = saisirBigBinaryAvecRetry();
        printf("Entrez le second nombre (B) :\n");
        BigBinary B = saisirBigBinaryAvecRetry();

        printf("=== PHASE 1 - FUNCTIONS DE BASE ===\n\n");

        printf("--- AFFICHAGE BINAIRE ---\n");
        printf("A = "); afficheBigBinary(A);
        printf("B = "); afficheBigBinary(B);
        if (A.Signe == -1 || B.Signe == -1) {
            printf("ERREUR : L'un des nombres ou les deux sont negatifs.\n");
            printf("Les operations de la Phase 1 et 2 ne sont supportees que pour les nombres positifs.\n");
            libereBigBinary(&A);
            libereBigBinary(&B);
            printf("\nvoulez-vous entrer d'autres nombres (o/n) ? :");
            scanf(" %c", &continuer);
            continue;
        }

        printf("\n--- COMPARAISONS ---\n");
        printf("A == B ? %s\n", egalBigBinary(&A, &B) ? "OUI" : "NON");
        printf("A < B ?  %s\n", inferieurBigBinary(&A, &B) ? "OUI" : "NON");
        printf("B < A ?  %s\n", inferieurBigBinary(&B, &A) ? "OUI" : "NON");
        printf("\n");

        printf("--- OPERATIONS ARITHMETIQUES ---\n");
        // Addition
        BigBinary S = additionBigBinary(&A, &B);
        printf("A + B = "); afficheBigBinary(S);
        libereBigBinary(&S);

        // Soustraction (seulement si A >= B)
        if (egalBigBinary(&A, &B)) {
            printf("A - B = 0 (A et B sont egaux)\n");
        } else if (inferieurBigBinary(&A, &B)) {
            printf("A - B = Operation impossible : A < B\n");
        } else {
            BigBinary D = soustractionBigBinary(&A, &B);
            printf("A - B = "); afficheBigBinary(D);
            libereBigBinary(&D);
        }

        printf("\n=== PHASE 2 - OPERATIONS AVANCEES ===\n");

        printf("\n--- Multiplication Egyptienne ---\n");
        BigBinary M = multiplicationEgyptienne(&A, &B);
        printf("A * B = "); afficheBigBinary(M);
        libereBigBinary(&M);

        printf("\n--- PGCD Binaire ---\n");
        BigBinary PGCD = pgcdBigBinary(&A, &B);
        printf("PGCD(A, B) = "); afficheBigBinary(PGCD);
        libereBigBinary(&PGCD);

        printf("\n--- Modulo sans division ---\n");
        if (inferieurBigBinary(&B, &A) || egalBigBinary(&A, &B)) {
            BigBinary MOD = moduloBigBinary(&A, &B);
            printf("A mod B = "); afficheBigBinary(MOD);
            libereBigBinary(&MOD);
        } else {
            printf("A mod B : A < B, donc A mod B = A\n");
        }

        printf("\n--- Exponentiation Modulaire ---\n");
        printf("Entrez un exposant e (entier decimal) : \n");
        printf(">");
        unsigned int e;
        scanf("%u", &e);

        // Pour l'exponentiation modulaire, on a besoin d'un module N
        printf("Entrez le module N :\n");
        BigBinary N = saisirBigBinaryAvecRetry();

        // Vérifier que A < N pour l'exponentiation modulaire
        if (inferieurBigBinary(&A, &N)) {
            BigBinary EXP = expModBigBinary(&A, e, &N);
            printf("A^%u mod N = ", e); afficheBigBinary(EXP);
            printf("%llu (decimal)\n", bigBinaryVersDecimal(&EXP));
            libereBigBinary(&EXP);
        } else {
            printf("A doit etre inferieur a N pour l'exponentiation modulaire\n");
        }

        // =========================================================
        // === PHASE 3 (BONUS) - RSA SIMPLIFIE ===
        // =========================================================
        printf("\n=== PHASE 3 (BONUS) - RSA SIMPLIFIE ===\n");

        // Définir les clés pour le test RSA (Exemple : N=1211809, E=101, D=251501)
        printf("\n--- Test Chiffrement/Dechiffrement RSA ---\n");
        printf("Entrez le message M a chiffrer (ex: 99999) :\n");
        BigBinary M_orig = saisirBigBinaryAvecRetry();

        printf("Entrez l'exposant public E (ex: 101) : \n");
        printf(">");
        unsigned int E_pub;
        scanf("%u", &E_pub);

        printf("Entrez l'exposant prive D (ex: 7) : \n");
        printf(">");
        unsigned int D_priv;
        scanf("%u", &D_priv);

        printf("Entrez le Module N (ex: 11100111) : \n");
        BigBinary N_rsa = saisirBigBinaryAvecRetry();

        // Chiffrement C = M^E mod N
        BigBinary C_chiffre = BigBinary_RSA_encrypt(&M_orig, E_pub, &N_rsa);
        printf("Message Chiffre C = M^E mod N :\n");
        printf("C (binaire) = "); afficheBigBinary(C_chiffre);
        printf("C (decimal) = %llu\n", bigBinaryVersDecimal(&C_chiffre));

        // Déchiffrement M_recupere = C^D mod N
        BigBinary M_recupere = BigBinary_RSA_decrypt(&C_chiffre, D_priv, &N_rsa);
        printf("\nMessage Dechiffre M = C^D mod N :\n");
        printf("M (binaire) = "); afficheBigBinary(M_recupere);
        printf("M (decimal) = %llu\n", bigBinaryVersDecimal(&M_recupere));

        // Vérification
        if (egalBigBinary(&M_orig, &M_recupere)) {
            printf("VERIFICATION: Chiffrement et Dechiffrement reussis.\n");
        } else {
            printf("ERREUR: Le message chiffre/dechiffre ne correspond pas au message original.\n");
        }

        // Libération mémoire spécifique au RSA
        libereBigBinary(&M_orig);
        libereBigBinary(&C_chiffre);
        libereBigBinary(&M_recupere);
        libereBigBinary(&N_rsa);

        // Libération mémoire finale (A et B du début de la boucle)
        libereBigBinary(&N);
        libereBigBinary(&A);
        libereBigBinary(&B);
        printf("\nvoulez-vous entrer d'autres nombres (o/n) ? :");
        scanf(" %c", &continuer);

    } while (continuer == 'o' || continuer == 'O' || continuer == 'oui');

    printf("\nfin du programme. Au revoir !\n");
    return 0;


}