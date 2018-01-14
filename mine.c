#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include "matrix_functions.h"
#include "keyboard_input.h"

#ifdef linux
#include "kbhit.h"
#endif

#ifdef _WIN32
#include<conio.h>
#endif

#define DARSTELLUNG_MINE 77 /* Der char "M" */
#define DARSTELLUNG_LEER 48 /* Der char "0" */

typedef struct Cursor {
        unsigned short int x;
        unsigned short int y;

    } Cursor;

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("TOO FEW ARGUMENTS!\nSyntax: mine rows columns mines\nexample: mine 8 8 4\n");
        return 1;
    }

    _Bool GameOver = 0;
    _Bool redrawScreen = 1;
    int rows, cols, mines, i;
    int **sichtbaresFeld;
    int **minenFeld;

    srand(time(NULL)); /* only needed for the shitty RNG, maybe replace later */

    rows = atoi(argv[1]);
    cols = atoi(argv[2]);
    mines = atoi(argv[3]);

    Cursor cursor;
    cursor.x = cols / 2 + 1;
    cursor.y = rows / 2;

    init_keyboard();

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
        /* clears the screen */
        if (redrawScreen == 1) {
            printf("%c[2J", 27);
            /* print the array */
            print_matrix(sichtbaresFeld, rows, cols);
            if (argc > 4) {
                printf("Minenfeld:\n");
                print_matrix(minenFeld, rows, cols);
            }
            printf("\nCursor:\nx: %d\ny: %d", cursor.x, cursor.y);
        }
        redrawScreen = 0;

        /* place cursor after drawing playing field */
        move_Cursor(cursor.y, cursor.x * 2);

        /*while (1) { */
            char c = 0;
            int countToThree = 0;

            c = getchar();
            if (c == 27 && getchar() == 91) {
                c = getchar();
                switch (c) {
                    case 65: cursor.y--; break;
                    case 66: cursor.y++; break;
                    case 67: cursor.x++; break;
                    case 68: cursor.x--; break;
                    default: printf("FEHLEINGABE !\n"); break;
                }
            } else if (c == 32) {
                if (check_matrixField(minenFeld, cursor.y - 1, cursor.x - 1) == DARSTELLUNG_MINE) {
                    GameOver = 1;
                } else if (check_matrixField(sichtbaresFeld, cursor.y - 1, cursor.x - 1) != DARSTELLUNG_LEER) {
                    printf("You've already checked this field!");
                } else if (check_matrixField(minenFeld, cursor.y - 1, cursor.x - 1) != 48) {
                    change_matrix(sichtbaresFeld, cursor.y - 1, cursor.x - 1, check_matrixField(minenFeld, cursor.y - 1, cursor.x - 1));
                } else {
                    reveal_sichtbaresFeld(sichtbaresFeld, minenFeld, rows, cols, cursor.y - 1, cursor.x - 1);
                }
                redrawScreen = 1;
            } else {
                printf("FEHLEINGABE !\n");
            }
    }

    /* clears the screen */
    printf("%c[2J", 27);
    print_matrix(minenFeld, rows, cols);
    printf("GAME OVER! You hit a mine!\n");

    close_keyboard();

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