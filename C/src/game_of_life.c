#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include "../include/game_of_life.h"

#define MIN_WAIT_TIME_IN_US 50000

#define CELL_FIELD_POS_X 1
#define CELL_FIELD_POS_Y 1
#define CELL_FIELD_HEIGHT 25
#define CELL_FIELD_WIDTH 80

#define MENU_POS_X 1
#define MENU_POS_Y 1
#define MENU_HEIGHT 3
#define MENU_WIDTH 80

#define COLOR_CELL_HAS_TOO_FEW_NEIGHBORS 1
#define COLOR_CELL_HAS_ENOUGH_NEIGHBORS 2
#define COLOR_CELL_HAS_TOO_MANY_NEIGHBORS 3
#define COLOR_CELL_IS_DEAD 4
#define COLOR_CELL_FIELD 5

#define COLOR_MENU 6
#define COLOR_PROGRESS_BAR 7

#define GOOD_MIN_NUMBER_OF_NEIGHBORS 2
#define GOOD_MAX_NUMBER_OF_NEIGHBORS 3

int main() {
    int height = CELL_FIELD_HEIGHT;
    int width = CELL_FIELD_WIDTH;
    cell_type **matrix_1 = NULL;
    cell_type **matrix_2 = NULL;
    int result = 0;
    if (allocate_dynamic_memory(&matrix_1, height, width) || allocate_dynamic_memory(&matrix_2, height, width)) {
        result = 1;
    } else {
        result = input(matrix_1, height, width);
        calculate_neighbors_for_cells(matrix_1, height, width);
        if (result == 0) {
            live(matrix_1, matrix_2, height, width);
        } else {
            printf("Invalid input\n");
        }
    }
    free_dynamic_memory(matrix_1);
    free_dynamic_memory(matrix_2);
    return result;
}

int allocate_dynamic_memory(cell_type ***matrix, int height, int width) {
    int result = 0;
    *matrix = malloc(height * width * sizeof(cell_type) + height* sizeof(cell_type *));
    if (*matrix == NULL) {
        printf("Failed to allocate memory\n");
        result = 1;
    } else {
        cell_type *p = (cell_type *) (*matrix + height);
        for (int i = 0; i < height; i++) {
            (*matrix)[i] = p + width * i;
        }
    }
    return result;
}

void calculate_neighbors_for_cells(cell_type **matrix, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            matrix[i][j].number_of_neighbors = get_number_of_living_neighbors(matrix, height, width, i, j);
        }
    }
}

double calculate_sleep_time(double speed_portion) {
    double sleep_time = MIN_WAIT_TIME_IN_US / 0.1;
    if (speed_portion > 0) {
        sleep_time = MIN_WAIT_TIME_IN_US / speed_portion;
    }
    return sleep_time;
}

WINDOW * create_window(int width, int height, int pos_x, int pos_y, int COLOR, bool is_colors_available) {
    WINDOW *window = newwin(height, width, pos_y, pos_x);
    if (is_colors_available) {
        wbkgd(window, COLOR_PAIR(COLOR));
    }
    return window;
}

double decrease_speed(double speed_portion) {
    return speed_portion - 0.1 >= 0.0 ? speed_portion - 0.1 : 0.0;
}

void free_dynamic_memory(void *pointer) {
    if (pointer != NULL) {
        free(pointer);
        pointer = NULL;
    }
}

int get_number_of_living_neighbors(cell_type **matrix, int height, int width, int row, int column) {
    int number_of_live = 0;
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = column - 1; j <= column + 1; j++) {
            int i_to_check;
            if (i < 0) {
                i_to_check = height - 1;
            } else if (i >= height) {
                i_to_check = 0;
            } else {
                i_to_check = i;
            }
            int j_to_check;
            if (j < 0) {
                j_to_check = width - 1;
            } else if (j >= width) {
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

int get_total_number_of_living_cells(cell_type **matrix, int height, int width) {
    int count = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (matrix[i][j].state) {
                count++;
            }
        }
    }
    return count;
}

double increase_speed(double speed_portion) {
    return speed_portion + 0.1 <= 1.0 ? speed_portion + 0.1 : 1.0;
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
        
        init_pair(COLOR_MENU, COLOR_BLACK, COLOR_WHITE);
        init_pair(COLOR_PROGRESS_BAR, COLOR_WHITE, COLOR_BLUE);
    }
    return colors_available;
}

int input(cell_type **matrix, int height, int width) {
    int result = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
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

int interact_with_user(double *speed_portion, double *sleep_time_in_us) {
    int stop = 0;
    int ch = getch();
    if (ch != ERR) {
        if (ch == '+') {
            *speed_portion = increase_speed(*speed_portion);
        } else if (ch == '-') {
            *speed_portion = decrease_speed(*speed_portion);
        } else if (ch == 'q' || ch == 'Q') {
            stop = 1;
        } else if (ch == 's' || ch == 'S') {
            
        }
        *sleep_time_in_us = calculate_sleep_time(*speed_portion);
    }
    return stop;
}

void live(cell_type **matrix_1, cell_type **matrix_2, int height, int width) {
    int turn = 1;
    cell_type ***matrix_source = &matrix_1;
    cell_type ***matrix_target = NULL;
    long time_to_show = get_time_in_us();
    initscr();
    cbreak();
    nodelay(stdscr, TRUE);
    noecho();
    bool is_colors_available = initialize_colors();
    WINDOW *window_cell_field = create_window(width, height, CELL_FIELD_POS_X, MENU_POS_Y + MENU_HEIGHT + CELL_FIELD_POS_Y, COLOR_CELL_FIELD, is_colors_available);
    WINDOW *window_menu = create_window(MENU_WIDTH, MENU_HEIGHT, MENU_POS_X, MENU_POS_Y, COLOR_MENU, is_colors_available);
    int to_exit = 0;
    double speed_portion = 0.5;
    double sleep_time_in_us = calculate_sleep_time(speed_portion);
    while (to_exit == 0) {
        long current_time = get_time_in_us();
        if (speed_portion != 0.0 && current_time - time_to_show >= sleep_time_in_us) {
            time_to_show = current_time;
            output(window_cell_field, window_menu, *matrix_source, height, width, speed_portion, is_colors_available);
            matrix_source = turn == 1 ? &matrix_1 : &matrix_2;
            matrix_target = turn == 1 ? &matrix_2 : &matrix_1;
            update_matrix(*matrix_source, *matrix_target, height, width);
            calculate_neighbors_for_cells(*matrix_target, height, width);
            matrix_source = matrix_target;
            if (turn == 1) {
                turn = 2;
            } else {
                turn = 1;
            }
        } else if (speed_portion == 0.0 && current_time - time_to_show >= sleep_time_in_us) {
            time_to_show = current_time;
            output(window_cell_field, window_menu, *matrix_source, height, width, speed_portion, is_colors_available);
        }
        if (interact_with_user(&speed_portion, &sleep_time_in_us)) {
            to_exit = 1;
        }
        if (get_total_number_of_living_cells(matrix_1, height, width) == 0) {
            to_exit = 2;
        }
    }
    endwin();
}

void output(WINDOW *window_cell_field, WINDOW *window_menu, cell_type **matrix, int n, int m, double speed_portion, bool is_colors_available) {
    output_menu(window_menu, speed_portion, is_colors_available);
    output_field_of_cells(window_cell_field, matrix, n, m, is_colors_available);
    doupdate();
}

void output_field_of_cells(WINDOW *window, cell_type **matrix, int height, int width, bool is_colors_available) {
    werase(window);
    wmove(window, 0, 0);
    if (is_colors_available) {
        output_field_of_cells_with_colors(window, matrix, height, width);
    } else {
        output_field_of_cells_without_colors(window, matrix, height, width);
    }
    wnoutrefresh(window);
}

void output_field_of_cells_with_colors(WINDOW *window, cell_type **matrix, int height, int width) {
    chtype cell;
    for (int i = 0; i < height; i++) {
        wmove(window, i, 0);
        for (int j = 0; j < width; j++) {
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

void output_field_of_cells_without_colors(WINDOW *window, cell_type **matrix, int height, int width) {
    for (int i = 0; i < height; i++) {
        wmove(window, i, 0);
        for (int j = 0; j < width; j++) {
            if (matrix[i][j].state == 0) {
                wprintw(window, " ");
            } else {
                wprintw(window, "0");
            }
        }
    }
}

void output_menu(WINDOW *window, double speed_portion, bool is_colors_available) {
    werase(window);
    wmove(window, 0, 0);
    wprintw(window, "To change speed, press + or - keys ");
    output_speed_bar(window, speed_portion, is_colors_available);
    wmove(window, 1, 0);
    wprintw(window, "To save cell field state, print s");
    wmove(window, 2, 0);
    wprintw(window, "To exit, print q");
    wnoutrefresh(window);
}

void output_speed_bar(WINDOW *window, double speed_portion, bool is_colors_availables) {
    if (is_colors_availables) {
        output_speed_bar_with_colors(window, speed_portion);
    } else {
        output_speed_bar_without_colors(window, speed_portion);
    }
}

void output_speed_bar_with_colors(WINDOW *window, double speed_portion) {
    double speed = 0.1;
    while (speed <= speed_portion) {
        waddch(window, ' ' | COLOR_PAIR(COLOR_PROGRESS_BAR));
        speed += 0.1;
    }
    while (speed < 1.0) {
        waddch(window, ' ' | COLOR_PAIR(COLOR_MENU));
        speed += 0.1;
    }
    wprintw(window, " %.1f%%", 100 * speed_portion);
}

void output_speed_bar_without_colors(WINDOW *window, double speed_portion) {
    double speed = 0.0;
    while (speed < speed_portion) {
        wprintw(window, "*");
        speed += 0.1;
    }
    while (speed < 1.0) {
        wprintw(window, " ");
        speed += 0.1;
    }
    wprintw(window, " %.1f%%", 100 * speed_portion);
}

void update_matrix(cell_type **matrix_source, cell_type **matrix_target, int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int number_of_living_neighbors = get_number_of_living_neighbors(matrix_source, height, width, i, j);
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
