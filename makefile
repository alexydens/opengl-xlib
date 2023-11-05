CFLAGS = -ansi -pedantic -Wall -Wextra -O3 -ffast-math
LDFLAGS = -Wl,--copy-dt-needed-entries -lABC -lGL -lGLU -lX11

SRC_DIR=src
BUILD_DIR=build

$(BUILD_DIR)/main: $(SRC_DIR)/main.c 
	gcc $(CFLAGS) $(SRC_DIR)/*.c -o $(BUILD_DIR)/main $(LDFLAGS)

$(BUILD_DIR)/main_test: $(SRC_DIR)/main.c
	gcc $(CFLAGS) $(SRC_DIR)/*.c -o $(BUILD_DIR)/main_test\
		$(LDFLAGS) -DDEBUG

$(BUILD_DIR)/main_gdb: $(SRC_DIR)/main.c
	gcc -g $(CFLAGS) $(SRC_DIR)/*.c -o $(BUILD_DIR)/main_gdb\
		$(LDFLAGS) -DDEBUG

.PHONY: run test build gdb clean

run: $(BUILD_DIR)/main
	$(BUILD_DIR)/main
	rm -rf $(BUILD_DIR)/main

test: $(BUILD_DIR)/main_test
	$(BUILD_DIR)/main_test
	rm -rf $(BUILD_DIR)/main_test

build: $(BUILD_DIR)/main
	mkdir $(BUILD_DIR)/assets
	cp assets/* build/assets

gdb: $(BUILD_DIR)/main_gdb
	gdb $(BUILD_DIR)/main_gdb

clean:
	rm -rf $(BUILD_DIR)/*
