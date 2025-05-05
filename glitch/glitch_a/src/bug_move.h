#ifndef BUG_MOVE_H
#define BUG_MOVE_H

#include "raylib.h"

#define MAX_LINES 128
#define MAX_LINE_LENGTH 1024
#define MAX_MOVED_WORDS 100
#define LINE_HEIGHT 40

extern char *lines[MAX_LINES];
extern int lineCount;

void LoadTextFile(const char *filename);
void ChooseWordsToMove(int percentage);
void UpdateMovedWords(float deltaTime);
void DrawTextWithBug(Font font, float deltaTime, float scrollOffset);

#endif

