#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myheader.h"

SUDOKU s_table;

// функція ініціалізує екземпляр структури
void init(char* path) {
    int pathlen = strlen(path);
    char data_fname[] = DATA_FNAME;
    while (path[pathlen - 1] != '\\' && path[pathlen - 1] != '/') pathlen--;

    char* datapath = malloc(sizeof(char) * (pathlen + strlen(data_fname)));
    if (!datapath) { printf(" Memory cannot be allocated"); return; }
    for (int i = 0; i < pathlen; ++i) {
        datapath[i] = path[i];
    }
    for (int j = strlen(data_fname); j > 0; --j) {
        datapath[pathlen + j] = data_fname[j];
    }








    s_table.size = 9;
    s_table.hidden = 0;
    int configuration[] = {0, 3, 6, 1, 4, 7, 2, 5, 8}; // перший стовпчик базової таблиці
    for (int i = 0; i < s_table.size; ++i) {
        for (int j = 0; j < s_table.size; ++j) {
            s_table.table[i][j] = (configuration[i] + j) % s_table.size + 1;
        }
    }
}
/* базове поле
 1 2 3 | 4 5 6 | 7 8 9
 4 5 6 | 7 8 9 | 1 2 3
 7 8 9 | 1 2 3 | 4 5 6
-------+-------+-------
 2 3 4 | 5 6 7 | 8 9 1
 5 6 7 | 8 9 1 | 2 3 4
 8 9 1 | 2 3 4 | 5 6 7
-------+-------+-------
 3 4 5 | 6 7 8 | 9 1 2
 6 7 8 | 9 1 2 | 3 4 5
 9 1 2 | 3 4 5 | 6 7 8
 */

// функція друкує таблицю із заголовком
void draw_table(int grid[9][9]) {
    printf("\n      S U D O K U \n\n");
    for (int i = 0; i < s_table.size; ++i) {
        for (int j = 0; j < s_table.size; ++j) {
            if (grid[i][j] == 0) printf("  ");
            else printf("%2d", grid[i][j]);
            if (j == 2 || j == 5) printf(" |") ;
        }
        (i == 2 || i == 5) ? printf("\n-------+-------+-------\n") : putchar('\n');
    }
}

// функція генерує таблицю - вибирає випадковий метод і перетасовує;
// повторяє тасування MIN_MIX від до MAX_MIX разів;
// при тасуванні таблиця залишається в стані, який задовільняє правила;
void generate_table() {
    int type;
    for (int i = rand() % (MAX_MIX - MIN_MIX) + MIN_MIX; i > 0; --i) {
        type = rand() % 5;
        switch (type) {
            case 0: transposing(); break;
            case 1: swap_rows_small(); break;
            case 2: swap_colums_small(); break;
            case 3: swap_rows_area(); break;
            case 4: swap_colums_area(); break;
            default: break;
        }
    }
}

// метод тасування - транспонування
void transposing(){
    for (int i = 0; i < s_table.size; ++i) {
        for (int j = i; j < s_table.size; ++j) {
            swap(&s_table.table[i][j], &s_table.table[j][i]);
        }
    }
}

// метод тасування - обмін двух рядків всередині блоку
void swap_rows_small(){
    int area = rand() % 3, line1 = rand() % 3, line2;
    do {
        line2 = rand() % 3;
    } while (line1 == line2);
    line1 = area * 3 + line1;
    line2 = area * 3 + line2;
    for (int i = 0; i < s_table.size; ++i) {
        swap(&s_table.table[line1][i], &s_table.table[line2][i]);
    }
}

// метод тасування - обмін двух стовпців всередині блоку
void swap_colums_small(){
    int area = rand() % 3, col1 = rand() % 3, col2;
    do {
        col2 = rand() % 3;
    } while (col1 == col2);
    col1 = area * 3 + col1;
    col2 = area * 3 + col2;
    for (int i = 0; i < s_table.size; ++i) {
        swap(&s_table.table[i][col1], &s_table.table[i][col2]);
    }
}

// метод тасування - обмін двух блоків по горизонталі
void swap_rows_area(){
    int area1 = rand() % 3 * 3, area2;
    do {
        area2 = rand() % 3 * 3;
    } while (area1 == area2);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < s_table.size; ++j) {
            swap(&s_table.table[area1 + i][j], &s_table.table[area2 + i][j]);
        }
    }
}

// метод тасування - обмін двух блоків по вертикалі
void swap_colums_area(){
    int area1 = rand() % 3 * 3, area2;
    do {
        area2 = rand() % 3 * 3;
    } while (area1 == area2);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < s_table.size; ++j) {
            swap(&s_table.table[j][area1 + i], &s_table.table[j][area2 + i]);
        }
    }
}

// функція ховає випадкові клітинки
void hide_numbers(){
    int a, b, isseen[s_table.size][s_table.size], ab_value, remains = s_table.size * s_table.size;
    for (int i = 0; i < s_table.size; ++i) { // копіюємо згенеровану таблицю
        for (int j = 0; j < s_table.size; ++j) {
            isseen[i][j] = 0;
            s_table.player_table[i][j] = s_table.table[i][j];
        }
    }
    while (remains) {  // поки є непереглянуті клітинки
        a = rand() % s_table.size; // випадково вибираємо клітинку
        b = rand() % s_table.size;
        if (isseen[a][b] == 0){ // якщо ця клітинка не переглянута,
            isseen[a][b] = 1; // то позначаємо як переглянуту
            remains--;
            ab_value = s_table.player_table[a][b]; // зберігаємо це значення
            s_table.player_table[a][b] = 0; // заховуємо цю клітинку
            if (count_solution(0, 0) != 1) { // ражуємо скільки можливих роз'язків даної конфігурації є
                s_table.player_table[a][b] = ab_value; // якщо немає або є декілька, то вертаємо це значення назад в клітинку
            }
            else s_table.hidden++; // якшо 1 рішення, то збільшуємо кількість захованих клітинок
        }
    }
}

// функція рахує кількість;
// параметри: x - номер рядка, y - номер стовпця клітинки яку будемо перевіряти;
int count_solution(int x, int y){
    int count = 0;
    if (x >= s_table.size) count++; // якщо дойшли до кінця таблиця, то збільшуємо кількість розі'язків
    else if (s_table.player_table[x][y] == 0){ // якщо ця клітинка пуста
        int isrepeated;
        for (int k = 1; k <= 9; ++k) { // в циклі підставляємо в цю клітинку кожну цифру
            s_table.player_table[x][y] = k;
            isrepeated = 0; // покищо не повторюється
            for (int p = 0; p < 9; ++p) { // перевіряємо чи ця цифра вже є в рядку
                if (y != p && s_table.player_table[x][y] == s_table.player_table[x][p]) {
                    isrepeated = 1; break; } // якщо є то позначаємо і виходимо із циклу
            }
            if (isrepeated == 0){ // якшо в рядку не повториласью,
                for (int p = 0; p < s_table.size; ++p) { // то перевіряємо в стовпці
                    if (x != p && s_table.player_table[x][y] == s_table.player_table[p][y]) {
                        isrepeated = 1; break; } // якщо є то позначаємо і виходимо із циклу
                }
                if (isrepeated == 0){ // якщо в рядку не повторилась, то перевіряжмо в квадраті
                    int x_c = x / 3 * 3, y_c = y / 3 * 3; //
                    for (int p = 0; p < 3; ++p) {
                        for (int q = 0; q < 3; ++q) {
                            if ((x_c + p) != x && (y_c + q) != y && s_table.player_table[x][y] == s_table.player_table[x_c + p][y_c + q]) {
                                isrepeated = 1; break; } // якщо є то позначаємо і виходимо із циклу
                        }
                    }
                    if (isrepeated == 0) // якщо не повторилась, то перевіряємо наступну клітинку і додаємо кількість розв'язків до уже знайдених
                        count += count_solution(x + (y + 1) / 9, (y + 1) % 9);
                }
            }
        }
        s_table.player_table[x][y] = 0;  // на виході рекурсіїповертаємо в клітинку значення 0
    }
    else count += count_solution(x + (y + 1) / s_table.size, (y + 1) % s_table.size);  // якщо ця клітинка має якусь цифру, то переходимо до наступної клітинки
    return count;
}

// функція міняє місцями два числа
void swap(int* a, int* b) {
    int swapper = *a;
    *a = *b;
    *b = swapper;
    /*
    *a ^= *b; // XOR swap
    *b ^= *a; // *b = (*b) ^ (*a) = (*b) ^ (*a ^ *b)  = *a
    *a ^= *b; // *a = (*a) ^ (*b) = (*a ^ *b) ^ (*a) = *b
    */
}