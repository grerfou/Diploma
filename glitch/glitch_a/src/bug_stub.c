#include "bug_stub.h"
#include "text_loader.h" 
#include "raylib.h"
#include <stdlib.h>
#include <string.h>

#define MAX_GLITCH_BANDS 50
#define GLITCH_PROBABILITY 0.05f
#define BAND_WIDTH 70
#define WORD_SPACING 10
#define FONT_SIZE 24
#define TEXT_MARGIN_LEFT 100
#define TEXT_MARGIN_TOP 100
#define LINE_HEIGHT 40
#define COLOR_TEXT (Color){255, 255, 255, 255}
#define GLITCH_UPDATE_INTERVAL 0.01f

static float glitchOffset[MAX_GLITCH_BANDS];
static Color glitchColors[MAX_GLITCH_BANDS];
static bool applyGlitchFrame = true;
static float glitchTimer = 0.0f;
static bool glitchMask[MAX_LINES][64];


void InitStubBug(void) {
    for (int i = 0; i < MAX_GLITCH_BANDS; i++) {
        glitchOffset[i] = (float)(rand() % 100) / 100.0f;
        glitchColors[i] = (Color){
            100 + rand() % 156,
            100 + rand() % 156,
            100 + rand() % 156,
            130 + rand() % 126
        };
    }
}

void NoteStub(Font font, float scrollOffset){
  DrawText("Bug_03 (Stub)", 1200, 100, 30, RED);
}


void UpdateStubBug(float deltaTime) {
    glitchTimer += deltaTime;
    if (glitchTimer >= GLITCH_UPDATE_INTERVAL) {
        glitchTimer = 0.0f;

        // Génère une nouvelle sélection aléatoire de mots à glitcher
        for (int i = 0; i < lineCount; i++) {
            char *line = lines[i];
            char *tmp = strdup(line);
            int wordCount = 0;

            strtok(tmp, " \n");
            while (strtok(NULL, " \n")) wordCount++;

            free(tmp);

            for (int j = 0; j < 64; j++) {
                glitchMask[i][j] = ((float)rand() / RAND_MAX) < GLITCH_PROBABILITY;
            }
        }
    }
}

void DrawStubBug(Font font, float deltaTime, float scrollOffset) {
    for (int i = 0; i < lineCount; i++) {
        char *line = lines[i];
        char *tmp = strdup(line);
        char *tokens[64] = {0};
        int tokenCount = 0;

        char *word = strtok(tmp, " \n");
        while (word && tokenCount < 64) {
            tokens[tokenCount++] = strdup(word);
            word = strtok(NULL, " \n");
        }

        float xPos = TEXT_MARGIN_LEFT;
        float yPos = TEXT_MARGIN_TOP + i * LINE_HEIGHT + scrollOffset;

        for (int j = 0; j < tokenCount; j++) {
            if (!applyGlitchFrame || ((float)rand() / RAND_MAX) > GLITCH_PROBABILITY) {
                // Pas de glitch
                DrawTextEx(font, tokens[j], (Vector2){xPos, yPos}, FONT_SIZE, 2, WHITE);
            } else {
                // Glitch visuel : décalages, transparence, couleurs
                Vector2 offset = { (float)(rand() % 7 - 2), (float)(rand() % 5 - 1) };

                // Couleurs typiques VHS
                Color colors[3] = {
                    (Color){255, 0, 0, 80},
                    (Color){0, 255, 255, 80},
                    (Color){255, 255, 255, 255}
                };

                for (int k = 0; k < 3; k++) {
                    Vector2 pos = {
                        xPos + offset.x * k,
                        yPos + offset.y * k
                    };
                    DrawTextEx(font, tokens[j], pos, FONT_SIZE, 2, colors[k]);
                }
            }

            xPos += MeasureTextEx(font, tokens[j], FONT_SIZE, 2).x + WORD_SPACING;
            free(tokens[j]);
        }

        free(tmp);
    }
}
