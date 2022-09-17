#ifndef C_INCLUDE_GAME_OF_LIFE_H_
#define C_INCLUDE_GAME_OF_LIFE_H_

#include <ncurses.h>

#define HIGH 100000
#define MIDDLE 500000
#define LOW 1000000

#define HEIGHT 25
#define WIDTH 80

#define CELL_IS_DEAD 0
#define CELL_HAS_TOO_FEW_NEIGHBORS 1
#define CELL_HAS_ENOUGH_NEIGHBORS 2
#define CELL_HAS_TOO_MANY_NEIGHBORS 3

#define GOOD_MIN_NUMBER_OF_NEIGHBORS 2
#define GOOD_MAX_NUMBER_OF_NEIGHBORS 3

typedef struct {
    int state;
    int number_of_neighbors;
} cell_type;

int allocate_dynamic_memory(cell_type ***matrix, int n, int m);
void calculate_neighbors_for_cells(cell_type **matrix, int n, int m);
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
void output(cell_type **matrix, int n, int m, unsigned int sleep_time_in_us, bool is_colors_available);
void output_field_of_cells(cell_type **matrix, int n, int m, bool is_colors_available);
void output_field_of_cells_with_colors(cell_type **matrix, int n, int m);
void output_field_of_cells_without_colors(cell_type **matrix, int n, int m);
void print_menu(unsigned int sleep_time_in_us);
void update_matrix(cell_type **matrix_source, cell_type **matrix_target, int n, int m);

#endif  // C_INCLUDE_GAME_OF_LIFE_H_
