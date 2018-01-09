int get_int_len (int value);
void populate_matrix(int** array, int rows, int cols, int mines);
void print_matrix(int** array, int rows, int cols);
void change_matrix(int** array, int change_row, int change_col, int newValue);
int check_matrixField(int** array, int row, int col);
void reveal_sichtbaresFeld(int** array, int** minenfeld, int rows, int cols, int x, int y);