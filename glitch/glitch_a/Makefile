CC = gcc
CFLAGS = -Wall -std=c99 -O2
LDFLAGS = -lraylib -lm

SRC_DIR = src
ASSETS_DIR = assets
BIN = main

SRC = $(SRC_DIR)/main.c $(SRC_DIR)/bug_move.c

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)

