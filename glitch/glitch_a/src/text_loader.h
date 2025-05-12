#ifndef TEXT_LOADER_H
#define TEXT_LOADER_H

#define MAX_LINES 128
#define MAX_LINE_LENGTH 1024
#define LINE_HEIGHT 40

extern char *lines[MAX_LINES];
extern int lineCount;

void LoadTextFromFile(const char *filename);

#endif

