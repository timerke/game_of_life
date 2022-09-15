SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := obj

EXE := $(BUILD_DIR)/game_of_life
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

CC := gcc
CFLAGS := -Wall -Werror -Wextra
LDFLAGS := -lncurses

all: $(EXE)

.PHONY: all clean

clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR)

$(EXE): $(OBJ) | $(BUILD_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR) $(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

