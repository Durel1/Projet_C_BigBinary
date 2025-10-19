#include <stdio.h>
#include "bigbinary.h"
#include "bigbinary.c"

int main() {
    printf("=== BIBLIOTHEQUE BIG BINARY ===\n");

    printf("Entrez le premier nombre (A) :\n");
    BigBinary A = saisirBigBinaryAvecRetry();

    printf("Entrez le second nombre (B) :\n");
    BigBinary B = saisirBigBinaryAvecRetry();

    printf("=== REPRESENTATION BINAIRE ===\n");
    printf("A = "); afficheBigBinary(A);
    printf("B = "); afficheBigBinary(B);

    printf("\n=== COMPARAISONS ===\n");
    printf("A == B ? %s\n", egalBigBinary(&A, &B) ? "OUI" : "NON");
    printf("A < B ?  %s\n", inferieurBigBinary(&A, &B) ? "OUI" : "NON");
    printf("B < A ?  %s\n", inferieurBigBinary(&B, &A) ? "OUI" : "NON");
    printf("\n");

    printf("=== OPERATIONS ARITHMETIQUES ===\n");
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

    // Libération mémoire
    libereBigBinary(&A);
    libereBigBinary(&B);

    return 0;
}