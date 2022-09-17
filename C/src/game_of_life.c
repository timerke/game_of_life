#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include "../include/game_of_life.h"

int main() {
    int m = WIDTH;
    int n = HEIGHT;
    cell_type **matrix_1 = NULL;
    cell_type **matrix_2 = NULL;
    int result = 0;
    if (allocate_dynamic_memory(&matrix_1, n, m) || allocate_dynamic_memory(&matrix_2, n, m)) {
        result = 1;
    } else {
        result = input(matrix_1, n, m);
        calculate_neighbors_for_cells(matrix_1, n, m);
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

int allocate_dynamic_memory(cell_type ***matrix, int n, int m) {
    int result = 0;
    *matrix = malloc(n * m * sizeof(cell_type) + n * sizeof(cell_type *));
    if (*matrix == NULL) {
        printf("Failed to allocate memory\n");
        result = 1;
    } else {
        cell_type *p = (cell_type *) (*matrix + n);
        for (int i = 0; i < n; i++) {
            (*matrix)[i] = p + m * i;
        }
    }
    return result;
}

void calculate_neighbors_for_cells(cell_type **matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix[i][j].number_of_neighbors = get_number_of_living_neighbors(matrix, n, m, i, j);
        }
    }
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
        pointer = NULL;
    }
}

int get_number_of_living_neighbors(cell_type **matrix, int n, int m, int row, int column) {
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
            if ((row != i_to_check || column != j_to_check) && matrix[i_to_check][j_to_check].state) {
                number_of_live++;
            }
        }
    }
    return number_of_live;
}

long get_time_in_us(void) {
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    return current_time.tv_sec * (int)1e6 + current_time.tv_usec;
}

int get_total_number_of_living_cells(cell_type **matrix, int n, int m) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (matrix[i][j].state) {
                count++;
            }
        }
    }
    return count;
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

bool initialize_colors(void) {
    bool colors_available = has_colors();
    if (colors_available) {
        start_color();
        init_pair(CELL_IS_DEAD, COLOR_WHITE, COLOR_WHITE);
        init_pair(CELL_HAS_TOO_FEW_NEIGHBORS, COLOR_WHITE, COLOR_GREEN);
        init_pair(CELL_HAS_ENOUGH_NEIGHBORS, COLOR_WHITE, COLOR_YELLOW);
        init_pair(CELL_HAS_TOO_MANY_NEIGHBORS, COLOR_WHITE, COLOR_RED);
    }
    return colors_available;
}

int input(cell_type **matrix, int n, int m) {
    int result = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (scanf("%d", &matrix[i][j].state) != 1) {
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

void live(cell_type **matrix_1, cell_type **matrix_2, int n, int m, unsigned int *sleep_time_in_us) {
    int turn = 1;
    cell_type ***matrix_source = &matrix_1;
    cell_type ***matrix_target = NULL;
    long time_to_show = get_time_in_us();
    initscr();
    cbreak();
    nodelay(stdscr, TRUE);
    noecho();
    bool is_colors_available = initialize_colors();
    int to_exit = 0;
    while (to_exit == 0) {
        long current_time = get_time_in_us();
        if (current_time - time_to_show >= *sleep_time_in_us) {
            time_to_show = current_time;
            output(*matrix_source, n, m, *sleep_time_in_us, is_colors_available);
            matrix_source = turn == 1 ? &matrix_1 : &matrix_2;
            matrix_target = turn == 1 ? &matrix_2 : &matrix_1;
            update_matrix(*matrix_source, *matrix_target, n, m);
            calculate_neighbors_for_cells(*matrix_target, n, m);
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
        if (get_total_number_of_living_cells(matrix_1, n, m) == 0) {
            to_exit = 2;
        }
    }
    endwin();
}

void output(cell_type **matrix, int n, int m, unsigned int sleep_time_in_us, bool is_colors_available) {
    clear();
    move(0, 0);
    output_field_of_cells(matrix, n, m, is_colors_available);
    print_menu(sleep_time_in_us);
    refresh();
}

void output_field_of_cells(cell_type **matrix, int n, int m, bool is_colors_available) {
    if (is_colors_available) {
        output_field_of_cells_with_colors(matrix, n, m);
    } else {
        output_field_of_cells_without_colors(matrix, n, m);
    }
}

void output_field_of_cells_with_colors(cell_type **matrix, int n, int m) {
    chtype cell;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (matrix[i][j].state == 0) {
                cell = ' ' | COLOR_PAIR(CELL_IS_DEAD);
            } else if (matrix[i][j].number_of_neighbors < GOOD_MIN_NUMBER_OF_NEIGHBORS) {
                cell = ' ' | COLOR_PAIR(CELL_HAS_TOO_FEW_NEIGHBORS);
            } else if (matrix[i][j].number_of_neighbors > GOOD_MAX_NUMBER_OF_NEIGHBORS) {
                cell = ' ' | COLOR_PAIR(CELL_HAS_TOO_MANY_NEIGHBORS);
            } else {
                cell = ' ' | COLOR_PAIR(CELL_HAS_ENOUGH_NEIGHBORS);
            }
            addch(cell);
        }
        printw("\n");
    }
}

void output_field_of_cells_without_colors(cell_type **matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (matrix[i][j].state == 0) {
                printw(" ");
            } else {
                printw("0");
            }
        }
        printw("\n");
    }
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

void update_matrix(cell_type **matrix_source, cell_type **matrix_target, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int number_of_living_neighbors = get_number_of_living_neighbors(matrix_source, n, m, i, j);
            if (matrix_source[i][j].state && (matrix_source[i][j].number_of_neighbors > GOOD_MAX_NUMBER_OF_NEIGHBORS ||
                                              matrix_source[i][j].number_of_neighbors < GOOD_MIN_NUMBER_OF_NEIGHBORS)) {
                matrix_target[i][j].state = 0;
            } else if (matrix_source[i][j].state == 0 && number_of_living_neighbors == GOOD_MAX_NUMBER_OF_NEIGHBORS) {
                matrix_target[i][j].state = 1;
            } else {
                matrix_target[i][j].state = matrix_source[i][j].state;
            }
        }
    }
}
