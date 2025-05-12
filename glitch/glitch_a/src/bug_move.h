#ifndef BUG_GLITCH_H
#define BUG_GLITCH_H

#include "raylib.h"

#define MAX_LINES 128
#define MAX_LINE_LENGTH 1024
#define MAX_MOVED_WORDS 100
#define LINE_HEIGHT 40

//extern char *lines[MAX_LINES];
//extern int lineCount;

void InitGlitchBug(void);
void NoteGlitch(Font font, float scrollOffset);
void ChooseLettersToMove(int percentage);
void UpdateGlitchBug(float deltaTime);
void DrawGlitchBug(Font font, float scrollOffset);

#endif

