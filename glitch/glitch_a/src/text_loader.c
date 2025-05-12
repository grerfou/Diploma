#include "text_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void LoadTextFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur ouverture texte");
        exit(1);
    }

    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, MAX_LINE_LENGTH, file)) {
        if (lineCount >= MAX_LINES) break;
        lines[lineCount] = strdup(buffer);
        lineCount++;
    }

    fclose(file);
}

