#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "myheader.h"

extern SUDOKU s_table;
static int message_index = 0; // змінні для виводу повідомлень
static char message1[] = "\n You cannot edit a cell (_; _)";
static char message2[] = "\n The number _ cannot be in the cell (_; _)";


int main(int argc, char* argv[]) {
    srand(time(NULL));





    init(argv[0]); // створюємо поле
    generate_table();
    hide_numbers(); // ховаємо деякі символи
    int pos_x, pos_y, value; // змінні, в які записуються введені користувачем значення

    while (s_table.hidden > 0){ // головний цикл; повторюємо поки не заповнимо всі клітинки
        clear(); // очищаємо екран

        draw_table(s_table.player_table); // друкуємо поле і повідомлення
        print_message();

        pos_x = -1, pos_y = -1, value = -1;
        printf("\n Enter the row number [1; %d] or 0 if you want to close the game : ", s_table.size);
        while (scanf("%d", & pos_x) != 1 || pos_x < 0 || pos_x > s_table.size) { // вводимо номер рядка і перевіряємо на правильність введення
            my_rewind();
            printf(" Enter the row number again [1; %d]: ", s_table.size);
        }
        if (pos_x == 0){ // якщо ввели цифру 0, то перевіряємо чи користувач точно бажає вийти з програми
            my_rewind();
            printf("\n Are you sure? \t[y/n]:   ");
            int end = getchar();
            if (end == 'y' || end == 'Y') break;
            else continue;
        }

        printf(" Enter the column number [1; %d] or 0 if you want to re-enter the row number: ", s_table.size);
        while (scanf("%d", & pos_y) != 1 || pos_y < 0 || pos_y > s_table.size) { // вводимо номер стовпця і перевіряємо на правильність введення
            my_rewind();
            printf(" Enter the column number again [1; %d]: ", s_table.size);
        }
        if (pos_y == 0) continue; // якщо ввели 0, то переходимо на наступну ітерацію циклу - вводимо номер рядка

        printf(" Enter a value [1; %d] to write to this cell or 0 if you want to re-enter the row number: ", s_table.size);
        while (scanf("%d", & value) != 1 || value < 0 || value > s_table.size) { // вводимо цисло яке будем записувати в обрану клітинку і перевіряємо на правильність введення
            my_rewind();
            printf(" Enter value again [1; %d]: ", s_table.size);
        }
        if (value == 0) continue; // якщо ввели 0, то переходимо на наступну ітерацію циклу - вводимо номер рядка

        change_value(pos_x-1, pos_y-1, value); // опрацьовуємо введені дані
    }

    if (s_table.hidden <= 0) { // якщо вийшли з циклу і все поле заповнено, то виводимо на екран відповідне повідомлення
        clear();
        draw_table(s_table.player_table);
        printf("\n    Congratulations!\n You have correctly decided Sudoku!!!\n\n"
               " (press any key to end the program)\n");
        getchar();
    }
    return 0;
}

// функція перевіряє чи отримане значення можна записати в дану клітинку і вказує яке повідомлення виводити потім
void change_value(int x, int y, int value) {
    if (s_table.player_table[x][y] == 0 ){ // якщо ця клітинка пуста
        if (s_table.table[x][y] == value) { // і значення правильне,
            s_table.player_table[x][y] = value; // то заповнюємо пусте місце цим значенням
            s_table.hidden--; // і зменшуємо кількість пустих клітинок
            message_index = 0;
        }
        else { // якщо в цій клітинці має бути інше числа, то виводимо відповідне повідомлення
            message2[38] = x+1 + '0';
            message2[41] = y+1 + '0';
            message2[13] = value + '0';
            message_index = 2;
        }
    }
    else { // якщо ця клітинка не пуста, то виводимо повідомлення що ми не можемо редагувати дану крітинку
        message1[26] = x+1 + '0';
        message1[29] = y+1 + '0';
        message_index = 1;
    }
}

// функція, що друкує потрібне повідомлення
void print_message() {
    switch (message_index) {
        case 1: puts(message1); break;
        case 2: puts(message2); break;
        default: break;
    }
}

// функція, що очищає екран
void clear() {
#if defined _WIN32
    system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
#elif defined (__APPLE__)
    system("clear");
#endif
}

// функція, що вручну очищає буфер введення
void my_rewind(){
    int c;
    while( (c = getchar()) != '\n' && c != EOF);
}
