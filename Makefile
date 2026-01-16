# CC = gcc
# CFLAGS = -Wall -Wextra -std=c99 -Ibuild
# LEMON = ./tools/lemon

# SRC_DIR = src
# BUILD_DIR = build
# SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/lexer.c $(BUILD_DIR)/xon.c
# TARGET = xon

# all: $(TARGET)

# $(BUILD_DIR)/xon.c: $(SRC_DIR)/xon.lemon
#     @mkdir -p $(BUILD_DIR)
#     $(LEMON) $(SRC_DIR)/xon.lemon
#     @mv xon.c xon.h xon.out $(BUILD_DIR)/ 2>/dev/null || true

# $(TARGET): $(BUILD_DIR)/xon.c $(SOURCES)
#     $(CC) $(CFLAGS) -o $(TARGET) $(SRC_DIR)/main.c $(SRC_DIR)/lexer.c

# clean:
#     rm -rf $(BUILD_DIR)/* $(TARGET)

# test: $(TARGET)
#     @echo "Testing basic example..."
#     ./$(TARGET) tests/test.xon
#     @echo "\nTesting config example..."
#     ./$(TARGET) examples/config.xon

# .PHONY: all clean test
