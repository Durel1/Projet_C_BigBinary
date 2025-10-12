#include <stdio.h>
#include "bigbinary.h"
#include "bigbinary.c"

int main() {
    printf("=== CALCULATRICE BIG BINARY ===\n");

    // afficherInstructions();

    // Saisie du premier nombre
    printf("Entrez le premier nombre (A) :\n");
    BigBinary A = saisirBigBinaryAvecRetry();

    printf("\nEntrez le second nombre (B) :\n");
    BigBinary B = saisirBigBinaryAvecRetry();

    printf("\n=== REPRESENTATION BINAIRE ===\n");
    printf("A = "); afficheBigBinary(A);
    printf("B = "); afficheBigBinary(B);

    printf("\n=== COMPARAISONS ===\n");
    printf("A == B ? %s\n", egalBigBinary(&A, &B) ? "OUI" : "NON");
    printf("A < B ?  %s\n", inferieurBigBinary(&A, &B) ? "OUI" : "NON");
    printf("B < A ?  %s\n", inferieurBigBinary(&B, &A) ? "OUI" : "NON");

    printf("\n=== OPERATIONS ARITHMETIQUES ===\n");

    // Addition
    BigBinary S = additionBigBinary(&A, &B);
    printf("A + B = "); afficheBigBinary(S);

    // Soustraction
    if (egalBigBinary(&A, &B)) {
        printf("A - B = 0 (A et B sont egaux)\n");
    } else if (inferieurBigBinary(&A, &B)) {
        BigBinary D = soustractionBigBinary(&B, &A);
        printf("A - B = -"); afficheBigBinary(D);
        libereBigBinary(&D);
    } else {
        BigBinary D = soustractionBigBinary(&A, &B);
        printf("A - B = "); afficheBigBinary(D);
        libereBigBinary(&D);
    }

    // Libération mémoire
    libereBigBinary(&A);
    libereBigBinary(&B);
    libereBigBinary(&S);

    return 0;
}