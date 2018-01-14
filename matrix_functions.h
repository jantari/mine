typedef struct Minefield {
        unsigned int rows;
        unsigned int columns;
        unsigned int mines;
        int **field;

    } Minefield;

int get_int_len (int value);
void populate_matrix(Minefield m);
void print_matrix(Minefield m);
void print_matrix_with_border(Minefield m);
void change_matrix(Minefield m, int change_row, int change_col, int newValue);
int check_matrixField(Minefield m, int row, int col);
void reveal_sichtbaresFeld(Minefield sichtbar, Minefield minenfeld, int x, int y);