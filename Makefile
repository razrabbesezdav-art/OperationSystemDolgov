CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -Werror -Iinclude
SANITIZE_FLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/args.c $(SRC_DIR)/proc_info.c $(SRC_DIR)/output.c
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
TARGET = $(BIN_DIR)/procview

.PHONY: all debug release run clean

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

debug: CFLAGS += -g -O0 $(SANITIZE_FLAGS)
debug: clean $(TARGET)

release: CFLAGS += -O2
release: clean $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)