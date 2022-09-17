#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include "../include/game_of_life.h"

#define HIGH 50000
#define MIDDLE 500000
#define LOW 1000000

#define CELL_FIELD_POS_X 1
#define CELL_FIELD_POS_Y 1
#define CELL_FIELD_HEIGHT 25
#define CELL_FIELD_WIDTH 80

#define MENU_POS_X 1
#define MENU_POS_Y 1
#define MENU_HEIGHT 5
#define MENU_WIDTH 80

#define COLOR_CELL_HAS_TOO_FEW_NEIGHBORS 1
#define COLOR_CELL_HAS_ENOUGH_NEIGHBORS 2
#define COLOR_CELL_HAS_TOO_MANY_NEIGHBORS 3
#define COLOR_CELL_IS_DEAD 4
#define COLOR_CELL_FIELD 5
#define COLOR_MENU 6

#define GOOD_MIN_NUMBER_OF_NEIGHBORS 2
#define GOOD_MAX_NUMBER_OF_NEIGHBORS 3

int main() {
    int m = CELL_FIELD_WIDTH;
    int n = CELL_FIELD_HEIGHT;
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

WINDOW * create_window(int width, int height, int pos_x, int pos_y, int COLOR, bool is_colors_available) {
    WINDOW *window = newwin(height, width, pos_y, pos_x);
    if (is_colors_available) {
        wbkgd(window, COLOR_PAIR(COLOR));
    }
    return window;
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
        init_pair(COLOR_CELL_IS_DEAD, COLOR_WHITE, COLOR_WHITE);
        init_pair(COLOR_CELL_HAS_TOO_FEW_NEIGHBORS, COLOR_WHITE, COLOR_GREEN);
        init_pair(COLOR_CELL_HAS_ENOUGH_NEIGHBORS, COLOR_WHITE, COLOR_YELLOW);
        init_pair(COLOR_CELL_HAS_TOO_MANY_NEIGHBORS, COLOR_WHITE, COLOR_RED);
        init_pair(COLOR_CELL_FIELD, COLOR_WHITE, COLOR_WHITE);
        init_pair(COLOR_MENU, COLOR_BLACK, COLOR_BLUE);
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
    WINDOW *window_cell_field = create_window(m, n, CELL_FIELD_POS_X, CELL_FIELD_POS_Y, COLOR_CELL_FIELD, is_colors_available);
    WINDOW *window_menu = create_window(MENU_WIDTH, MENU_HEIGHT, MENU_POS_X, CELL_FIELD_POS_Y + n + MENU_POS_Y,
                                        COLOR_MENU, is_colors_available);
    int to_exit = 0;
    while (to_exit == 0) {
        long current_time = get_time_in_us();
        if (current_time - time_to_show >= *sleep_time_in_us) {
            time_to_show = current_time;
            output(window_cell_field, window_menu, *matrix_source, n, m, *sleep_time_in_us, is_colors_available);
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

void output(WINDOW *window_cell_field, WINDOW *window_menu, cell_type **matrix, int n, int m, unsigned int sleep_time_in_us, bool is_colors_available) {
    output_field_of_cells(window_cell_field, matrix, n, m, is_colors_available);
    output_menu(window_menu, sleep_time_in_us);
    doupdate();
}

void output_field_of_cells(WINDOW *window, cell_type **matrix, int n, int m, bool is_colors_available) {
    werase(window);
    wmove(window, 0, 0);
    if (is_colors_available) {
        output_field_of_cells_with_colors(window, matrix, n, m);
    } else {
        output_field_of_cells_without_colors(window, matrix, n, m);
    }
    wnoutrefresh(window);
}

void output_field_of_cells_with_colors(WINDOW *window, cell_type **matrix, int n, int m) {
    chtype cell;
    for (int i = 0; i < n; i++) {
        wmove(window, i, 0);
        for (int j = 0; j < m; j++) {
            if (matrix[i][j].state == 0) {
                cell = ' ' | COLOR_PAIR(COLOR_CELL_IS_DEAD);
            } else if (matrix[i][j].number_of_neighbors < GOOD_MIN_NUMBER_OF_NEIGHBORS) {
                cell = ' ' | COLOR_PAIR(COLOR_CELL_HAS_TOO_FEW_NEIGHBORS);
            } else if (matrix[i][j].number_of_neighbors > GOOD_MAX_NUMBER_OF_NEIGHBORS) {
                cell = ' ' | COLOR_PAIR(COLOR_CELL_HAS_TOO_MANY_NEIGHBORS);
            } else {
                cell = ' ' | COLOR_PAIR(COLOR_CELL_HAS_ENOUGH_NEIGHBORS);
            }
            waddch(window, cell);
        }
    }
}

void output_field_of_cells_without_colors(WINDOW *window, cell_type **matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        wmove(window, i, 0);
        for (int j = 0; j < m; j++) {
            if (matrix[i][j].state == 0) {
                wprintw(window, " ");
            } else {
                wprintw(window, "0");
            }
        }
    }
}

void output_menu(WINDOW *window, unsigned int sleep_time_in_us) {
    werase(window);
    wmove(window, 0, 0);
    wprintw(window, "To change speed, press + or - keys ");
    switch (sleep_time_in_us) {
        case LOW:
            wprintw(window, "(current speed is LOW)");
            break;
        case MIDDLE:
            wprintw(window, "(current speed is MIDDLE)");
            break;
        case HIGH:
            wprintw(window, "(current speed is HIGH)");
            break;
    }
    wmove(window, 1, 0);
    wprintw(window, "To exit, print q");
    wnoutrefresh(window);
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
