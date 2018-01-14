#include<stdio.h>
#include"keyboard_input.h"

void move_Cursor(int row, int col) {
    printf("%c[%d;%dH", 27, row, col - 1);
}
