#include<time.h> /* only needed for the shitty RNG, maybe replace later */
#include<stdio.h>
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
void populate_matrix(int** array, int rows, int cols, int mines) {
    int i, j;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            array[i][j] = 48;
        }
    }

    /* placing mines and numbers in playing field */
    for (int minesPlaced = 0; minesPlaced < mines;) {
        int randomRow = rand() % rows;
        int randomCol = rand() % cols;
        /* printf("Cell randomly selected: X: %d Y: %d\n", randomRow, randomCol); */
        if (array[randomRow][randomCol] != DARSTELLUNG_MINE) {
            array[randomRow][randomCol] = DARSTELLUNG_MINE;
            /* Zahlen hinzufuegen wieviele Minen in der naehe sind */
            for (int startRow = randomRow - 1; startRow <= randomRow + 1; startRow++) {
                for (int startCol = randomCol - 1; startCol <= randomCol + 1; startCol++) {
                    /* Check ob der Bereich noch im Spielfeld ist (falls Mine am Rand ist) */
                    if (startRow < rows && startRow >= 0 && startCol < cols && startCol >= 0) {
                        /* Check ob Feld bereits mine ist, wenn ja ueberspringen */
                        if (array[startRow][startCol] != DARSTELLUNG_MINE) {
                            array[startRow][startCol]++;
                        }
                    }
                }
            }
            minesPlaced++;
        }
    }
    
}

void print_matrix(int** array, int rows, int cols) {
    printf("    ");

    for (int i = 0; i < cols; i++) {
        printf("_%c[4m%d%c[0m", 27, i, 27);
    }
    printf("_\n");
    for (int i = 0; i < rows; i++) {
        for (int padding = 0; padding + get_int_len(i) < get_int_len(rows); padding++) {
            printf(" ");
        }
        printf("%d | ", i);
        for (int j = 0; j < cols; j++) {
            if (array[i][j] == 77) {
                printf("%c[31m%c%c[0m ", 27, array[i][j], 27);
            } else if (array[i][j] == 48) {
                printf("+ ");
            } else {
                printf("%c ", array[i][j]);
            }
        }
        printf("|\n");
    }
}


void change_matrix(int** array, int change_row, int change_col, int newValue) {
    array[change_row][change_col] = newValue;
}


int check_matrixField(int** array, int row, int col) {
    return array[row][col];
}

void reveal_sichtbaresFeld(int** array, int** minenfeld, int rows, int cols, int x, int y) {

    if (minenfeld[x][y] != 48) {
        array[x][y] = minenfeld[x][y];
    } else {
        array[x][y] = 32;

        if (REVEAL_ADJACENT_ZEROS == 1) {

        for (int startRow = x - 1; startRow <= x + 1; startRow++) {
            if (DEBUG_OUTPUT == 1) { printf("\nStartRow: %d", startRow); }
            for (int startCol = y - 1; startCol <= y + 1; startCol++) {
                if (DEBUG_OUTPUT == 1) { printf("\nStartRow: %d, StartCol: %d", startRow, startCol); }
                /* Check ob der Bereich noch im Spielfeld ist (falls Mine am Rand ist) */
                if (startRow < rows && startRow >= 0 && startCol < cols && startCol >= 0) {
                    /* Check ob Feld bereits mine ist, wenn ja ueberspringen */
                    if (DEBUG_OUTPUT == 1) { printf(" - checked"); }
                        if (array[startRow][startCol] != 32) {
                            /* Funktion wird fuer die aktuelle Position nur erneut aufgerufen wenn */
                            /* entweder REVEAL_ADJACENT_OTHER 1 ist oder wenn es 0 ist (ausgeschaltet UND */
                            /* die aktuelle Position eine 0 ist. */
                            if (REVEAL_ADJACENT_OTHER == 0 && minenfeld[startRow][startCol] == 48 || REVEAL_ADJACENT_OTHER == 1)
                            reveal_sichtbaresFeld(array, minenfeld, rows, cols, startRow, startCol);
                        }
                }
            }
        }
    }
    }
}