CC = gcc
CFLAGS = -std=c11 -g -Wall -Wextra -Iinclude
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
TEST_LD_FLAGS= -Iinclude

BIN_DIR= bin
BUILD_DIR= build
OUT=$(BIN_DIR)/scaff
TEST_OUT= $(BUILD_DIR)/test

SRCS = $(wildcard src/**/*.c) $(wildcard src/*.c)
TESTS= $(wildcard tests/*.c) $(wildcard src/**/*.c)

$(OUT):$(SRCS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRCS) -o $(OUT) $(LDFLAGS)
	
$(TEST_OUT):$(TESTS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(TESTS) -o $(TEST_OUT) $(TEST_LD_FLAGS)
	
test:$(TEST_OUT)
	./$(TEST_OUT)
	

run:$(OUT)
	./$(OUT)
	
.PHONY:clean	
	
clean:
	rm -rf $(BIN_DIR)
