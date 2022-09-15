#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define HIGH 100000
#define MIDDLE 500000
#define LOW 1000000

#define HEIGHT 25
#define WIDTH 80

int allocate_dynamic_memory(int ***matrix, int n, int m);
void decrease_speed(unsigned int *sleep_time_in_us);
void free_dynamic_memory(void *pointer);
int get_number_of_live(int **matrix, int n, int m, int row, int column);
long get_time_in_us();
void increase_speed(unsigned int *sleep_time_in_us);
int input(int **matrix, int n, int m);
int interact_with_user(unsigned int *sleep_time_in_us);
int is_alive(int **matrix, int n, int m);
void live(int **matrix_1, int **matrix_2, int n, int m, unsigned int *sleep_time_in_us);
void output(int **matrix, int n, int m, unsigned int sleep_time_in_us);
void print_menu(unsigned int sleep_time_in_us);
void update_matrix(int **matrix_source, int **matrix_target, int n, int m);

int main() {
    int m = WIDTH;
    int n = HEIGHT;
    int **matrix_1;
    int **matrix_2;
    int result = 0;
    if (allocate_dynamic_memory(&matrix_1, n, m) || allocate_dynamic_memory(&matrix_2, n, m)) {
        result = 1;
    } else {
        result = input(matrix_1, n, m);
        if (result == 0) {
            unsigned int sleep_time = MIDDLE;
            live(matrix_1, matrix_2, n, m, &sleep_time);
        } else {
            printf("Invalid input\n");
        }
    }
    free_dynamic_memory(matrix_1);
    free_dynamic_memory(matrix_2);
    return result;
}

int allocate_dynamic_memory(int ***matrix, int n, int m) {
    int result = 0;
    *matrix = malloc(n * m * sizeof(int) + n * sizeof(int *));
    if (*matrix == NULL) {
        printf("Failed to allocate memory\n");
        result = 1;
    } else {
        int *p = (int *) (*matrix + n);
        for (int i = 0; i < n; i++) {
            (*matrix)[i] = p + m * i;
        }
    }
    return result;
}

void decrease_speed(unsigned int *sleep_time_in_us) {
    switch (*sleep_time_in_us) {
        case HIGH:
            *sleep_time_in_us = MIDDLE;
            break;
        case MIDDLE:
            *sleep_time_in_us = LOW;
            break;
    }
}

void free_dynamic_memory(void *pointer) {
    if (pointer != NULL) {
        free(pointer);
    }
}

int get_number_of_live(int **matrix, int n, int m, int row, int column) {
    int number_of_live = 0;
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = column - 1; j <= column + 1; j++) {
            int i_to_check;
            if (i < 0) {
                i_to_check = n - 1;
            } else if (i >= n) {
                i_to_check = 0;
            } else {
                i_to_check = i;
            }
            int j_to_check;
            if (j < 0) {
                j_to_check = m - 1;
            } else if (j >= m) {
                j_to_check = 0;
            } else {
                j_to_check = j;
            }
            if ((row != i_to_check || column != j_to_check) && matrix[i_to_check][j_to_check]) {
                number_of_live++;
            }
        }
    }
    return number_of_live;
}

long get_time_in_us() {
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    return current_time.tv_sec * (int)1e6 + current_time.tv_usec;
}

void increase_speed(unsigned int *sleep_time_in_us) {
    switch (*sleep_time_in_us) {
        case LOW:
            *sleep_time_in_us = MIDDLE;
            break;
        case MIDDLE:
            *sleep_time_in_us = HIGH;
            break;
    }
}

int input(int **matrix, int n, int m) {
    int result = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                result = 1;
                break;
            }
        }
    }
    if (freopen("/dev/tty", "r", stdin) == NULL) {
        result = 1;
    }
    return result;
}

int interact_with_user(unsigned int *sleep_time_in_us) {
    int stop = 0;
    int ch = getch();
    if (ch != ERR) {
        if (ch == '+') {
            increase_speed(sleep_time_in_us);
        } else if (ch == '-') {
            decrease_speed(sleep_time_in_us);
        } else if (ch == 'q' || ch == 'Q') {
            stop = 1;
        }
    }
    return stop;
}

int is_alive(int **matrix, int n, int m) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (matrix[i][j] == 1) {
                count++;
            }
        }
    }
    return count;
}

void live(int **matrix_1, int **matrix_2, int n, int m, unsigned int *sleep_time_in_us) {
    int turn = 1;
    int ***matrix_source = &matrix_1;
    int ***matrix_target = NULL;
    long time_to_show = 0;
    time(&time_to_show);
    initscr();
    cbreak();
    nodelay(stdscr, TRUE);
    int to_exit = 0;
    while (to_exit == 0) {
        long current_time = get_time_in_us();
        if (current_time - time_to_show >= *sleep_time_in_us) {
            time_to_show = current_time;
            output(*matrix_source, n, m, *sleep_time_in_us);
            matrix_source = turn == 1 ? &matrix_1 : &matrix_2;
            matrix_target = turn == 1 ? &matrix_2 : &matrix_1;
            update_matrix(*matrix_source, *matrix_target, n, m);
            matrix_source = matrix_target;
            if (turn == 1) {
                turn = 2;
            } else {
                turn = 1;
            }
        }
        if (interact_with_user(sleep_time_in_us)) {
            to_exit = 1;
        }
        if (is_alive(matrix_1, n, m) == 0) {
            to_exit = 2;
        }
    }
    endwin();
}

void output(int **matrix, int n, int m, unsigned int sleep_time_in_us) {
    clear();
    move(0, 0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (matrix[i][j] == 0) {
                printw(" ");
            } else {
                printw("0");
            }
        }
        printw("\n");
    }
    print_menu(sleep_time_in_us);
}

void print_menu(unsigned int sleep_time_in_us) {
    printw("To change speed, press + or - keys ");
    switch (sleep_time_in_us) {
        case LOW:
            printw("(current speed is LOW)");
            break;
        case MIDDLE:
            printw("(current speed is MIDDLE)");
            break;
        case HIGH:
            printw("(current speed is HIGH)");
            break;
    }
    printw("\nTo exit, print q");
}

void update_matrix(int **matrix_source, int **matrix_target, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int number_of_live = get_number_of_live(matrix_source, n, m, i, j);
            if (matrix_source[i][j] == 1 && (number_of_live > 3 || number_of_live < 2)) {
                matrix_target[i][j] = 0;
            } else if (matrix_source[i][j] == 0 && (number_of_live == 3)) {
                matrix_target[i][j] = 1;
            } else {
                matrix_target[i][j] = matrix_source[i][j];
            }
        }
    }
}

