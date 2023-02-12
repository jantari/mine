typedef struct Minefield {
    unsigned int rows;
    unsigned int columns;
    unsigned int mines;
    int **field;
    int **mask;

} Minefield;

typedef struct Cursor {
    unsigned short int x;
    unsigned short int y;

} Cursor;

void move_Cursor(int row, int col);
int get_int_len (int value);
void populate_matrix(Minefield m);
void print_matrix(Minefield m, int** field);
void set_matrixField(int* m, int newValue);
int get_matrixField(int** m, int row, int col);
void reveal_mineField(Minefield m, int x, int y);
_Bool check_if_won(Minefield m);
void reveal_all_mines(Minefield m);
