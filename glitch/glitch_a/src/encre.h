#ifndef ENCRE_H
#define ENCRE_H

#include "raylib.h"

void InitInkBug();
void UpdateInkBug(float deltaTime);
void DrawTextWithInk(Font font, float scrollOffset);
void ChooseWordsToInk(int percentage);

#endif

