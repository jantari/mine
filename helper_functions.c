#include <stdio.h>
#include <stdlib.h> /* only needed for RAND */
#include <time.h> /* only needed for the shitty RNG, maybe replace later */
#include "helper_functions.h"

#define DARSTELLUNG_MINE 77 /* Der char "M" */
#define REVEAL_ADJACENT_ZEROS 1
#define REVEAL_ADJACENT_OTHER 1
#define DEBUG_OUTPUT 0

void move_Cursor(int row, int col) {
    printf("\x1b[%d;%dH", row, col - 1);
}

int get_int_len (int value){
    int len = 1;
    while(value > 9) {
        len++;
        value/=10;
    }
    return len;
}

void populate_matrix(Minefield m) {
    int i, j;

    for (i = 0; i < m.rows; i++) {
        for (j = 0; j < m.columns; j++) {
            m.field[i][j] = 48;
            m.mask[i][j] = 48;
        }
    }

    /* placing mines and numbers in playing field */
    for (int minesPlaced = 0; minesPlaced < m.mines;) {
        int randomRow = rand() % m.rows;
        int randomCol = rand() % m.columns;
        if (m.field[randomRow][randomCol] != DARSTELLUNG_MINE) {
            m.field[randomRow][randomCol] = DARSTELLUNG_MINE;
            /* Zahlen ins Feld einfuegen wieviele Minen in der Naehe sind */
            for (int startRow = randomRow - 1; startRow <= randomRow + 1; startRow++) {
                for (int startCol = randomCol - 1; startCol <= randomCol + 1; startCol++) {
                    /* Check ob der Bereich noch im Spielfeld ist (falls Mine am Rand ist) */
                    if (startRow < m.rows && startRow >= 0 && startCol < m.columns && startCol >= 0) {
                        /* Check ob Feld bereits mine ist, wenn ja ueberspringen */
                        if (m.field[startRow][startCol] != DARSTELLUNG_MINE) {
                            m.field[startRow][startCol]++;
                        }
                    }
                }
            }
            minesPlaced++;
        }
    }
}

void print_matrix(Minefield m, int** field) {
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.columns; j++) {
            if (field[i][j] == DARSTELLUNG_MINE) {
                printf("\x1b[31m%c\x1b[0m ", field[i][j]);
            } else if (field[i][j] == 48) {
                printf("+ ");
            } else {
                printf("%c ", field[i][j]);
            }
        }
        printf("|\n");
    }
}

void set_matrixField(int *m, int newValue) {
    *m = newValue;
}

int get_matrixField(int** m, int row, int col) {
    return m[row][col];
}

void reveal_mineField(Minefield m, int x, int y) {
    if (m.field[x][y] != 48) {
        m.mask[x][y] = m.field[x][y];
    } else {
        m.mask[x][y] = 32;

        if (REVEAL_ADJACENT_ZEROS == 1) {
            for (int startRow = x - 1; startRow <= x + 1; startRow++) {
                if (DEBUG_OUTPUT == 1) { printf("\nStartRow: %d", startRow); }
                for (int startCol = y - 1; startCol <= y + 1; startCol++) {
                    if (DEBUG_OUTPUT == 1) { printf("\nStartRow: %d, StartCol: %d", startRow, startCol); }
                    /* Check ob der Bereich noch im Spielfeld ist (falls Mine am Rand ist) */
                    if (startRow < m.rows && startRow >= 0 && startCol < m.columns && startCol >= 0) {
                        /* Check ob Feld bereits mine ist, wenn ja ueberspringen */
                        if (DEBUG_OUTPUT == 1) { printf(" - checked"); }
                        if (m.mask[startRow][startCol] != 32) {
                            /* Funktion wird fuer die aktuelle Position nur erneut aufgerufen wenn */
                            /* entweder REVEAL_ADJACENT_OTHER 1 ist oder wenn es 0 ist (ausgeschaltet UND */
                            /* die aktuelle Position eine 0 ist. */
                            if (REVEAL_ADJACENT_OTHER == 0 && m.field[startRow][startCol] == 48 || REVEAL_ADJACENT_OTHER == 1)
                            reveal_mineField(m, startRow, startCol);
                        }
                    }
                }
            }
        }
    }
}

/* returns 0 (false) if an unrevealed field that's not a mine is found in the mask */
/* and 1 (true) if all non-mine fields have been uncovered by the player */
_Bool check_if_won(Minefield m) {
    for (int currentRow = 0; currentRow < m.rows; currentRow++) {
        for (int currentColumn = 0; currentColumn < m.columns; currentColumn++) {
            if(m.mask[currentRow][currentColumn] == 48 && m.field[currentRow][currentColumn] != DARSTELLUNG_MINE) {
                return 0;
            }
        }
    }
    return 1;
}

void reveal_all_mines(Minefield m) {
    int i = 0, j = 0;

    for (i = 0; i < m.rows; i++) {
        for (j = 0; j < m.columns; j++) {
            if (m.field[i][j] == DARSTELLUNG_MINE) {
                m.mask[i][j] = DARSTELLUNG_MINE;
            }
        }
    }
}
