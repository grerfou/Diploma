CC = gcc
CFLAGS = -Wall -std=c99 -O2
LDFLAGS = -lraylib -lm

SRC_DIR = src
ASSETS_DIR = assets
BIN = main

SRC = $(SRC_DIR)/main.c $(SRC_DIR)/bug_move.c $(SRC_DIR)/text_loader.c $(SRC_DIR)/bug_stub.c $(SRC_DIR)/vhs_glitch.c 

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)

