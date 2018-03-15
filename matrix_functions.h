typedef struct Minefield {
        unsigned int rows;
        unsigned int columns;
        unsigned int mines;
        int **field;
        int **mask;

    } Minefield;

int get_int_len (int value);
void populate_matrix(Minefield m);
void print_matrix(Minefield m, int** field);
void print_matrix_with_border(Minefield m);
void change_matrix(int* m, int newValue);
int check_matrixField(int** m, int row, int col);
void reveal_minefield(Minefield m, int x, int y);
void reveal_all_mines(Minefield m);