#include<stdlib.h>
#include<stdio.h>
#include<time.h>

#include "matrix_functions.h"

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

void move_Cursor(int row, int col);

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("TOO FEW ARGUMENTS!\nSyntax: mine rows columns mines\nexample: mine 8 8 4\n");
        return 1;
    }

    _Bool GameOver = 0;
    _Bool redrawScreen = 1;
    int i;

    Minefield sichtbaresFeld;
    sichtbaresFeld.rows = atoi(argv[1]);
    sichtbaresFeld.columns = atoi(argv[2]);
    sichtbaresFeld.mines = 0;

    Minefield minenFeld;
    minenFeld.rows = sichtbaresFeld.rows;
    minenFeld.columns = sichtbaresFeld.columns;
    minenFeld.mines = atoi(argv[3]);

    srand(time(NULL)); /* only needed for the shitty RNG, maybe replace later */

    Cursor cursor;
    cursor.x = sichtbaresFeld.columns / 2 + 1;
    cursor.y = sichtbaresFeld.rows / 2;

    #ifdef linux
    init_keyboard();
    #endif

    /* allocate the arrays */
    sichtbaresFeld.field = malloc(sichtbaresFeld.rows * sizeof *sichtbaresFeld.field);
    for (i = 0; i < sichtbaresFeld.rows; i++) {
        sichtbaresFeld.field[i] = malloc(sichtbaresFeld.columns * sizeof *sichtbaresFeld.field[i]);
    }

    minenFeld.field = malloc(minenFeld.rows * sizeof *minenFeld.field);
    for (i = 0; i < minenFeld.rows; i++) {
        minenFeld.field[i] = malloc(minenFeld.columns * sizeof *minenFeld.field[i]);
    }

    /* use the array */
    populate_matrix(sichtbaresFeld);
    populate_matrix(minenFeld);

    while (GameOver == 0) {
        /* clears the screen */
        if (redrawScreen == 1) {
            printf("%c[2J", 27);
            /* print the array */
            print_matrix(sichtbaresFeld);
            if (argc > 4) {
                printf("Minenfeld:\n");
                print_matrix(minenFeld);
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
                    reveal_sichtbaresFeld(sichtbaresFeld, minenFeld, cursor.y - 1, cursor.x - 1);
                }
                redrawScreen = 1;
            } else {
                printf("FEHLEINGABE !\n");
            }
    }

    /* clears the screen */
    printf("%c[2J", 27);
    print_matrix(minenFeld);
    printf("GAME OVER! You hit a mine!\n");

    #ifdef linux
    close_keyboard();
    #endif
    
    /* deallocate the array */
    for (i = 0; i < sichtbaresFeld.rows; i++) {
        free(sichtbaresFeld.field[i]);
    }
    free(sichtbaresFeld.field);

    for (i = 0; i < minenFeld.rows; i++) {
        free(minenFeld.field[i]);
    }
    free(minenFeld.field);
}

void move_Cursor(int row, int col) {
    printf("%c[%d;%dH", 27, row, col - 1);
}
