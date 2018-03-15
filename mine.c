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
    srand(time(NULL)); /* only needed for the shitty RNG, maybe replace later */

    Minefield minefield;
    minefield.rows = atoi(argv[1]);
    minefield.columns = atoi(argv[2]);
    minefield.mines = atoi(argv[3]);
    /* allocate the arrays */
    minefield.field = malloc(minefield.rows * sizeof *minefield.field);
    minefield.mask = malloc(minefield.rows * sizeof *minefield.mask);
    for (i = 0; i < minefield.rows; i++) {
        minefield.mask[i] = malloc(minefield.columns * sizeof *minefield.mask[i]);
        minefield.field[i] = malloc(minefield.columns * sizeof *minefield.field[i]);
    }

    Cursor cursor;
    cursor.x = minefield.columns / 2 + 1;
    cursor.y = minefield.rows / 2;

    /* use the array */
    populate_matrix(minefield);

    #ifdef linux
    init_keyboard();
    #endif

    while (GameOver == 0) {
        /* clears the screen */
        if (redrawScreen == 1) {
            printf("%c[2J", 27);
            /* print the array */
            print_matrix(minefield, minefield.mask);
            if (argc > 4) {
                printf("Minenfeld:\n");
                print_matrix(minefield, minefield.field);
            }
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
                    case 65: if (cursor.y > 1) cursor.y--; break;
                    case 66: if (cursor.y < minefield.rows) cursor.y++; break;
                    case 67: if (cursor.x < minefield.columns) cursor.x++; break;
                    case 68: if (cursor.x > 1) cursor.x--; break;
                    default: printf("FEHLEINGABE !\n"); break;
                }
            } else if (c == 32) {
                if (check_matrixField(minefield.field, cursor.y - 1, cursor.x - 1) == DARSTELLUNG_MINE) {
                    GameOver = 1;
                } else if (check_matrixField(minefield.field, cursor.y - 1, cursor.x - 1) != 48) {
                    change_matrix(&minefield.mask[cursor.y - 1][cursor.x - 1], check_matrixField(minefield.field, cursor.y - 1, cursor.x - 1));
                } else {
                    reveal_minefield(minefield, cursor.y - 1, cursor.x - 1);
                }
                redrawScreen = 1;
            } else {
                printf("FEHLEINGABE !\n");
            }
    }

    /* clears the screen */
    printf("%c[2J", 27);
    reveal_all_mines(minefield);
    print_matrix(minefield, minefield.mask);
    printf("GAME OVER! You hit a mine!\n");

    #ifdef linux
    close_keyboard();
    #endif
    
    /* deallocate the array */
    for (i = 0; i < minefield.rows; i++) {
        free(minefield.field[i]);
        free(minefield.mask[i]);
    }
    free(minefield.field);
    free(minefield.mask);
}

void move_Cursor(int row, int col) {
    printf("%c[%d;%dH", 27, row, col - 1);
}
