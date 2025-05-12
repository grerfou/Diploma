// vhs_glitch.h

#ifndef VHS_GLITCH_H
#define VHS_GLITCH_H

#include "raylib.h"

extern Texture2D glitchTexture;

void InitVHSGlitch(void);
void NoteVHS(Font font, float scrollOffset);
void UpdateVHSGlitch(float deltaTime);
void DrawVHSGlitch(Font font, float deltaTime, float scrollOffset);

#endif // VHS_GLITCH_H

