# Nom du compilateur
CC = gcc

# Options de compilation – vous pouvez ajuster -O2 pour l'optimisation
CFLAGS = -O2 -Wall -Wextra -Iinclude

# Options de linkage (vérifiez que raylib et ses dépendances sont correctement installées)
LDFLAGS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

# Récupérer tous les fichiers sources dans src/
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

# Nom de l'exécutable
TARGET = cuneiform

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

