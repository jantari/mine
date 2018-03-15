#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "helper_functions.h"

#ifdef linux
#include "kbhit.h"
#endif

#ifdef _WIN32
#include <conio.h>
#endif

#define DARSTELLUNG_MINE 77 /* Der char "M" */

void quit_game(void);

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("TOO FEW ARGUMENTS!\nSyntax: mine rows columns mines\nexample: mine 8 8 4\n");
        return 1;
    }

    unsigned short int GameState = 1;
    /* GameState: 0 lost
                  1 ongoing
                  2 won
                  3 user quit */
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

    while (GameState == 1) {
        /* clears the screen */
        if (redrawScreen == 1) {
            printf("\x1b[2J");
            /* print the array */
            print_matrix(minefield, minefield.mask);
            if (argc > 4) {
                printf("Minenfeld:\n");
                print_matrix(minefield, minefield.field);
            }
            redrawScreen = 0;
        }

        /* place cursor after drawing playing field */
        move_Cursor(cursor.y, cursor.x * 2);

        char c = getchar();
        switch (c) {
            case 27: /* user pressed special key */
                if (getchar() == 91) {
                    c = getchar();
                    switch (c) {
                        case 65: if (cursor.y > 1) cursor.y--; break;
                        case 66: if (cursor.y < minefield.rows) cursor.y++; break;
                        case 67: if (cursor.x < minefield.columns) cursor.x++; break;
                        case 68: if (cursor.x > 1) cursor.x--; break;
                        default: printf("FEHLEINGABE !\n"); break;
                    }
                }
                break;
            case 32: /* user pressed spacebar */
                switch (check_matrixField(minefield.field, cursor.y - 1, cursor.x - 1)) {
                    case DARSTELLUNG_MINE: GameState = 0; break;
                    case 48: reveal_minefield(minefield, cursor.y - 1, cursor.x - 1); break;
                    default: change_matrix(&minefield.mask[cursor.y - 1][cursor.x - 1], check_matrixField(minefield.field, cursor.y - 1, cursor.x - 1)); break;
                }
                redrawScreen = 1;
                break;
            case 113: /* user pressed 'q' */
                GameState = 3;
        }
    }

    /* clears the screen */
    printf("\x1b[2J");

    if (GameState == 0) {
        reveal_all_mines(minefield);
        print_matrix(minefield, minefield.mask);
        printf("GAME OVER! You hit a mine!\n");
    }

    #ifdef linux
    close_keyboard();
    #endif

    /* deallocate the array */
    for (int i = 0; i < minefield.rows; i++) {
        free(minefield.field[i]);
        free(minefield.mask[i]);
    }
    free(minefield.field);
    free(minefield.mask);
}