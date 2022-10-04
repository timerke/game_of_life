#ifndef C_INCLUDE_GAME_OF_LIFE_H_
#define C_INCLUDE_GAME_OF_LIFE_H_

#include <ncurses.h>

typedef struct {
    int state;
    int number_of_neighbors;
} cell_type;

int allocate_dynamic_memory(cell_type ***matrix, int height, int width);
void calculate_neighbors_for_cells(cell_type **matrix, int height, int width);
double calculate_sleep_time(double speed_portion);
int check_terminal_arguments(int argc, char *argv[], int *height, int *width);
void copy_matrix(cell_type **matrix_source, int height_source, int width_source, cell_type **matrix_target, int height_target, int width_target);
WINDOW * create_window(int width, int height, int pos_x, int pos_y, int COLOR, bool is_colors_available);
double decrease_speed(double speed_portion);
void free_dynamic_memory(void *pointer);
void generate_random_initial_state(cell_type **matrix, int height, int width);
int get_number_of_living_neighbors(cell_type **matrix, int height, int width, int row, int column);
int get_random_size(int min_size, int max_size);
long get_time_in_us(void);
int get_total_number_of_living_cells(cell_type **matrix, int height, int width);
int increase_dynamic_memory(cell_type ***matrix, int *height, int *width, int what_to_increase);
double increase_speed(double speed_portion);
bool initialize_colors(void);
int input_initial_state(cell_type ***matrix, int *height, int *width);
int interact_with_user(double *speed_portion, double *sleep_time_in_us, cell_type **matrix, int height, int width);
void live(cell_type **matrix_1, cell_type **matrix_2, int height, int width);
void output(WINDOW *window_cell_field, WINDOW *window_menu, cell_type **matrix, int height, int width, double speed_portion, bool is_colors_available,
            int number_of_live_cells);
void output_field_of_cells(WINDOW *window, cell_type **matrix, int height, int width, bool is_colors_available);
void output_field_of_cells_with_colors(WINDOW *window, cell_type **matrix, int height, int width);
void output_field_of_cells_without_colors(WINDOW *window, cell_type **matrix, int height, int width);
void output_menu(WINDOW *window, double speed_portion, bool is_colors_available, int number_of_live_cells);
void output_speed_bar(WINDOW *window, double speed_portion, bool is_colors_available);
void output_speed_bar_with_colors(WINDOW *window, double speed_portion);
void output_speed_bar_without_colors(WINDOW *window, double speed_portion);
void save_matrix(cell_type **matrix, int height, int width);
int update_matrix(cell_type **matrix_source, cell_type **matrix_target, int height, int width);

#endif  // C_INCLUDE_GAME_OF_LIFE_H_
