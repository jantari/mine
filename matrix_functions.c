#include<stdio.h>
#include<stdlib.h>  /* only needed for RAND */
#include<time.h> /* only needed for the shitty RNG, maybe replace later */
#include "matrix_functions.h"

#define DARSTELLUNG_MINE 77 /* Der char "M" */
/* #define DARSTELLUNG_LEER 48  Der char "0" */
#define REVEAL_ADJACENT_ZEROS 1
#define REVEAL_ADJACENT_OTHER 1
#define DEBUG_OUTPUT 0

int get_int_len (int value){
    int len = 1;
    while(value > 9) {
        len++;
        value/=10;
    }
    return len;
}

/* Syntax: populate_matrix(matrixHier, anzahlReihen, anzahlSpalten, anzahlMinen) */
void populate_matrix(Minefield m) {
    int i, j;

    for (i = 0; i < m.rows; i++) {
        for (j = 0; j < m.columns; j++) {
            m.field[i][j] = 48;
        }
    }

    /* placing mines and numbers in playing field */
    for (int minesPlaced = 0; minesPlaced < m.mines;) {
        int randomRow = rand() % m.rows;
        int randomCol = rand() % m.columns;
        /* printf("Cell randomly selected: X: %d Y: %d\n", randomRow, randomCol); */
        if (m.field[randomRow][randomCol] != DARSTELLUNG_MINE) {
            m.field[randomRow][randomCol] = DARSTELLUNG_MINE;
            /* Zahlen hinzufuegen wieviele Minen in der naehe sind */
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

void print_matrix(Minefield m) {
    for (int i = 0; i < m.rows; i++) {
        for (int j = 0; j < m.columns; j++) {
            if (m.field[i][j] == 77) {
                printf("%c[31m%c%c[0m ", 27, m.field[i][j], 27);
            } else if (m.field[i][j] == 48) {
                printf("+ ");
            } else {
                printf("%c ", m.field[i][j]);
            }
        }
        printf("|\n");
    }
}

/* UNUSED NOW */
void print_matrix_with_border(Minefield m) {
    printf("    ");
    for (int i = 0; i < m.columns; i++) {
        printf("_%c[4m%d%c[0m", 27, i, 27);
    }
    printf("_\n");
    for (int i = 0; i < m.rows; i++) {
        for (int padding = 0; padding + get_int_len(i) < get_int_len(m.rows); padding++) {
            printf(" ");
        }
        printf("%d | ", i);
        for (int j = 0; j < m.columns; j++) {
            if (m.field[i][j] == 77) {
                printf("%c[31m%c%c[0m ", 27, m.field[i][j], 27);
            } else if (m.field[i][j] == 48) {
                printf("+ ");
            } else {
                printf("%c ", m.field[i][j]);
            }
        }
        printf("|\n");
    }
}


void change_matrix(Minefield m, int change_row, int change_col, int newValue) {
    m.field[change_row][change_col] = newValue;
}


int check_matrixField(Minefield m, int row, int col) {
    return m.field[row][col];
}

void reveal_sichtbaresFeld(Minefield sichtbar, Minefield minenfeld, int x, int y) {
    if (minenfeld.field[x][y] != 48) {
        sichtbar.field[x][y] = minenfeld.field[x][y];
    } else {
        sichtbar.field[x][y] = 32;

        if (REVEAL_ADJACENT_ZEROS == 1) {
            for (int startRow = x - 1; startRow <= x + 1; startRow++) {
                if (DEBUG_OUTPUT == 1) { printf("\nStartRow: %d", startRow); }
                for (int startCol = y - 1; startCol <= y + 1; startCol++) {
                    if (DEBUG_OUTPUT == 1) { printf("\nStartRow: %d, StartCol: %d", startRow, startCol); }
                    /* Check ob der Bereich noch im Spielfeld ist (falls Mine am Rand ist) */
                    if (startRow < sichtbar.rows && startRow >= 0 && startCol < sichtbar.columns && startCol >= 0) {
                        /* Check ob Feld bereits mine ist, wenn ja ueberspringen */
                        if (DEBUG_OUTPUT == 1) { printf(" - checked"); }
                        if (sichtbar.field[startRow][startCol] != 32) {
                            /* Funktion wird fuer die aktuelle Position nur erneut aufgerufen wenn */
                            /* entweder REVEAL_ADJACENT_OTHER 1 ist oder wenn es 0 ist (ausgeschaltet UND */
                            /* die aktuelle Position eine 0 ist. */
                            if (REVEAL_ADJACENT_OTHER == 0 && minenfeld.field[startRow][startCol] == 48 || REVEAL_ADJACENT_OTHER == 1)
                            reveal_sichtbaresFeld(sichtbar, minenfeld, startRow, startCol);
                        }
                    }
                }
            }
        }
    }
}

void reveal_all_mines(Minefield sichtbar, Minefield minenfeld) {
    int i = 0, j = 0;

    for (i = 0; i < minenfeld.rows; i++) {
        for (j = 0; j < minenfeld.columns; j++) {
            if (minenfeld.field[i][j] == DARSTELLUNG_MINE) {
                sichtbar.field[i][j] = DARSTELLUNG_MINE;
            }
        }
    }
}