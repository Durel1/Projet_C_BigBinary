#include <stdio.h>
#include "bigbinary.h"
#include "bigbinary.c"

int main() {
    printf("=== BIBLIOTHEQUE BIG BINARY === \n\n");
    printf("Entrez le premier nombre (A) :\n");
    BigBinary A = saisirBigBinaryAvecRetry();
    printf("Entrez le second nombre (B) :\n");
    BigBinary B = saisirBigBinaryAvecRetry();

    printf("=== PHASE 1 - FUNCTIONS DE BASE ===\n\n");
    printf("--- AFFICHAGE BINAIRE ---\n");
    printf("A = "); afficheBigBinary(A);
    printf("B = "); afficheBigBinary(B);

    printf("\n--- COMPARAISONS ---\n");
    printf("A == B ? %s\n", egalBigBinary(&A, &B) ? "OUI" : "NON");
    printf("A < B ?  %s\n", inferieurBigBinary(&A, &B) ? "OUI" : "NON");
    printf("B < A ?  %s\n", inferieurBigBinary(&B, &A) ? "OUI" : "NON");
    printf("\n");

    printf("--- OPERATIONS ARITHMETIQUES ---\n");
    // Addition (toujours possible avec des nombres positifs)
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
    printf("Entrez un exposant e (entier decimal) : ");
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

    libereBigBinary(&N);
    // Libération mémoire finale
    libereBigBinary(&A);
    libereBigBinary(&B);

    return 0;
}