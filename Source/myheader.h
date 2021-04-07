#ifndef SUDOKU_MYHEADER_H
#define SUDOKU_MYHEADER_H

#define MAX_MIX 75
#define MIN_MIX 55

typedef struct SUDOKU {
    int size;
    int hidden;
    int table[9][9];
    int player_table[9][9];
} SUDOKU;

void init();
void change_value(int x, int y, int value);
void print_message();
void draw_table();

void generate_table();
void transposing();
void swap_rows_small();
void swap_colums_small();
void swap_rows_area();
void swap_colums_area();

void hide_numbers();
int count_solution(int x, int y);

void swap(int* a, int* b);
void clear();
void my_rewind();

#endif //SUDOKU_MYHEADER_H


/* TO DO
 * додати олівець
 * якщо вийшов, то запам'ятовувати таблицю в файлі
   і перед початком запитувати чи продовжувати попередню гру
 * додати керування з клавіатури
 * додати кольори
 * покращити генерацію
 * додати лічильник помилок (також зберігати в файлі)
 * додати різні розміри (2, 3, 4, 5)
 */