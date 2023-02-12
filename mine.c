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
        printf("TOO FEW ARGUMENTS!\nSyntax: mine rows columns mines\nexample: mine 10 10 8\n");
        return 1;
    }

    enum { LOST, ONGOING, WON, QUIT } GameState = 1;
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
            /* moves cursor to 1:1 coordinates before redrawing to fix positioning bug with urxvt */
            move_Cursor(1, 1);
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

        #ifdef linux
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
        #endif
        #ifdef _WIN32
        char c = getch();
        switch (c) {
            case 0:
                switch (getch()) {
                    // Up
                    case 72: if (cursor.y > 1) cursor.y--; break;
                    // Down
                    case 80: if (cursor.y < minefield.rows) cursor.y++; break;
                    // Right
                    case 77: if (cursor.x < minefield.columns) cursor.x++; break;
                    // Left
                    case 75: if (cursor.x > 1) cursor.x--; break;
                    default: printf("FEHLEINGABE !\n"); break;
                }
                break;
        #endif
            case 32: /* user pressed spacebar */
                switch (get_matrixField(minefield.field, cursor.y - 1, cursor.x - 1)) {
                    case DARSTELLUNG_MINE: GameState = 0; break;
                    case 48: reveal_mineField(minefield, cursor.y - 1, cursor.x - 1); break;
                    default: set_matrixField(&minefield.mask[cursor.y - 1][cursor.x - 1], get_matrixField(minefield.field, cursor.y - 1, cursor.x - 1)); break;
                }
                if (check_if_won(minefield) == 1) {
                    GameState = 2;
                }
                redrawScreen = 1;
                break;
            case 113: /* user pressed 'q' */
                GameState = 3;
        }
    }

    /* clears the screen */
    printf("\x1b[2J");

    /* moves cursor to 1:1 coordinates before redrawing to fix positioning bug with urxvt */
    move_Cursor(1, 1);

    reveal_all_mines(minefield);
    print_matrix(minefield, minefield.mask);
    /* the following code neccessitates that GameState 0 means lost and any positive number means won */
    puts(GameState ? "YOU WON! All non-mine fields revealed!" : "GAME OVER! You hit a mine!");

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
