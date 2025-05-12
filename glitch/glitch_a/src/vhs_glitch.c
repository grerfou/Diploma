// vhs_glitch.c

#include "vhs_glitch.h"
#include "text_loader.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#define FONT_SIZE         30
#define WORD_SPACING      10
#define TEXT_MARGIN_LEFT  500
#define TEXT_MARGIN_TOP   0

#define MAX_GLITCH_BANDS 20  // Nombre de bandes glitch
#define BAND_WIDTH 20  // Largeur des bandes glitch
#define MAX_BANDS 16
#define MAX_TOKENS 64

static float glitchOffset[MAX_GLITCH_BANDS];  // Décalage des bandes glitch
//static Color glitchColors[MAX_GLITCH_BANDS];  // Couleur des bandes glitch
Color COLOR_TEXTE = {255, 255, 255, 255};
static float glitchSpeed = 5.0f;

typedef struct {
    int y;
    Color color;
} GlitchBand;

static GlitchBand bands[MAX_BANDS];

// Initialisation des bandes glitch
void InitVHSGlitch() {
    for (int i = 0; i < MAX_GLITCH_BANDS; i++) {
       glitchOffset[i] = (float)(rand() % 100) / 100.0f;  // Décalage aléatoire
    }
}

void NoteVHS(Font font, float scrollOffset) {
    DrawText("Bug VHS (VHS)", 100, 100, 30, RED);
}

void UpdateVHSGlitch(float delta) {
    static float glitchTimer = 0.0f;
    glitchTimer += delta * glitchSpeed;

    if (glitchTimer >= 0.1f) {  // met à jour toutes les 0.1s * vitesse
        for (int i = 0; i < MAX_BANDS; i++) {
            bands[i].y = rand() % (GetScreenHeight() + 1);
            bands[i].color = (Color){0, 0, 0, 255};
        }
        glitchTimer = 0.0f;
    }
}


void DrawVHSGlitch(Font font, float delta, float scrollOffset) {
    for (int i = 0; i < lineCount; i++) {
        char *line = lines[i];
        char *tmp = strdup(line);
        char *tokens[MAX_TOKENS] = {0};
        int tokenCount = 0;

        char *word = strtok(tmp, " \n");
        while (word && tokenCount < MAX_TOKENS) {
            tokens[tokenCount++] = strdup(word);
            word = strtok(NULL, " \n");
        }

        float xPos = TEXT_MARGIN_LEFT;
        float yPos = TEXT_MARGIN_TOP + i * LINE_HEIGHT + scrollOffset;

        for (int j = 0; j < tokenCount; j++) {
            Vector2 pos = { xPos, yPos };

            int len = strlen(tokens[j]);
            float charSpacing = 2.0f;
            float glyphWidth = MeasureTextEx(font, "A", FONT_SIZE, charSpacing).x;

            for (int c = 0; c < len; c++) {
                char ch[2] = { tokens[j][c], '\0' };

                // Déformation par jitter aléatoire
                //float xJitter = (rand() % 3) - 1;      // -1 à +1 pixels
                //float yJitter = (rand() % 5) - 2;      // -2 à +2 pixels

                Vector2 mainPos = { pos.x + c * glyphWidth, pos.y};

                //DrawTextEx(font, ch, mainPos, FONT_SIZE, charSpacing, COLOR_TEXTE);
                DrawTextEx(font, tokens[j], pos, FONT_SIZE, 2, (Color){255, 255, 255, 255});
            }

            xPos += MeasureTextEx(font, tokens[j], FONT_SIZE, charSpacing).x + WORD_SPACING;
            free(tokens[j]);
        }

        free(tmp);
    }

    // Glitchs horizontaux (bandes noires aléatoires)
    for (int i = 0; i < MAX_BANDS; i++) {
        DrawRectangle(0, bands[i].y, GetScreenWidth(), 2, BLACK);
    }
}

