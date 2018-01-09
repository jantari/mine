#include<stdlib.h>
#include<stdio.h>

#include "matrix_functions.h"

#define DARSTELLUNG_MINE 77 /* Der char "M" */
#define DARSTELLUNG_LEER 48 /* Der char "0" */

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("TOO FEW ARGUMENTS!\nSyntax: mine rows columns mines\nexample: mine 8 8 4\n");
        return 1;
    }

    _Bool GameOver = 0;
    int rows, cols, mines, i;
    int **sichtbaresFeld;
    int **minenFeld;

    srand(time(NULL)); /* only needed for the shitty RNG, maybe replace later */

    rows = atoi(argv[1]);
    cols = atoi(argv[2]);
    mines = atoi(argv[3]);

    int nutzereingabe1;
    int nutzereingabe2;

    /* allocate the arrays */
    sichtbaresFeld = malloc(rows * sizeof *sichtbaresFeld);
    for (i = 0; i < rows; i++) {
        sichtbaresFeld[i] = malloc(cols * sizeof *sichtbaresFeld[i]);
    }

    minenFeld = malloc(rows * sizeof *minenFeld);
    for (i = 0; i < rows; i++) {
        minenFeld[i] = malloc(cols * sizeof *minenFeld[i]);
    }

    /* use the array */
    populate_matrix(sichtbaresFeld, rows, cols, 0);
    populate_matrix(minenFeld, rows, cols, mines);

    while (GameOver == 0) {
        printf("%c[2J", 27);
        /* print the array */
        printf("Sichtbares Feld:\n");
        print_matrix(sichtbaresFeld, rows, cols);
        if (argc > 4) {
            printf("Minenfeld:\n");
            print_matrix(minenFeld, rows, cols);
        }

        do {
            printf("Coordinates where you want to check (row column): ");
            scanf("%d %d", &nutzereingabe1, &nutzereingabe2);
        } while (nutzereingabe1 >= rows || nutzereingabe2 >= cols);

        if (check_matrixField(minenFeld, nutzereingabe1, nutzereingabe2) == DARSTELLUNG_MINE) {
            GameOver = 1;
        } else if (check_matrixField(sichtbaresFeld, nutzereingabe1, nutzereingabe2) != DARSTELLUNG_LEER) {
            printf("You've already checked this field!");
        } else if (check_matrixField(minenFeld, nutzereingabe1, nutzereingabe2) != 48) {
            change_matrix(sichtbaresFeld, nutzereingabe1, nutzereingabe2, check_matrixField(minenFeld, nutzereingabe1, nutzereingabe2));
        } else {
            reveal_sichtbaresFeld(sichtbaresFeld, minenFeld, rows, cols, nutzereingabe1, nutzereingabe2);
        }
    }

    printf("GAME OVER! You hit a mine!\n");
    print_matrix(minenFeld, rows, cols);

    /* deallocate the array */
    for (i = 0; i < rows; i++) {
        free(sichtbaresFeld[i]);
    }
    free(sichtbaresFeld);

    for (i = 0; i < rows; i++) {
        free(minenFeld[i]);
    }
    free(minenFeld);
}