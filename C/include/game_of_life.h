#ifndef C_INCLUDE_GAME_OF_LIFE_H_
#define C_INCLUDE_GAME_OF_LIFE_H_

#include <ncurses.h>

typedef struct {
    int state;
    int number_of_neighbors;
} cell_type;

int allocate_dynamic_memory(cell_type ***matrix, int n, int m);
void calculate_neighbors_for_cells(cell_type **matrix, int n, int m);
WINDOW * create_window(int width, int height, int pos_x, int pos_y, int COLOR, bool is_colors_available);
void decrease_speed(unsigned int *sleep_time_in_us);
void free_dynamic_memory(void *pointer);
int get_number_of_living_neighbors(cell_type **matrix, int n, int m, int row, int column);
long get_time_in_us(void);
int get_total_number_of_living_cells(cell_type **matrix, int n, int m);
void increase_speed(unsigned int *sleep_time_in_us);
bool initialize_colors(void);
int input(cell_type **matrix, int n, int m);
int interact_with_user(unsigned int *sleep_time_in_us);
void live(cell_type **matrix_1, cell_type **matrix_2, int n, int m, unsigned int *sleep_time_in_us);
void output(WINDOW *window_cell_field, WINDOW *window_menu, cell_type **matrix, int n, int m, unsigned int sleep_time_in_us, bool is_colors_available);
void output_field_of_cells(WINDOW *window, cell_type **matrix, int n, int m, bool is_colors_available);
void output_field_of_cells_with_colors(WINDOW *window, cell_type **matrix, int n, int m);
void output_field_of_cells_without_colors(WINDOW *window, cell_type **matrix, int n, int m);
void output_menu(WINDOW *window, unsigned int sleep_time_in_us);
void update_matrix(cell_type **matrix_source, cell_type **matrix_target, int n, int m);

#endif  // C_INCLUDE_GAME_OF_LIFE_H_
