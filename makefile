# Compiler và flags
CC      := gcc

SRC_DIR := Source
BUILD_DIR := build
TARGET  := $(BUILD_DIR)/app

# Lấy tất cả thư mục con để thêm vào -I
INC_DIRS := $(shell find $(SRC_DIR) -type d)
CFLAGS  := -Wall -Wextra -O2 $(foreach dir,$(INC_DIRS),-I$(dir))

# Tìm tất cả các file .c
SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Rule chính
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

# Compile .c -> .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
