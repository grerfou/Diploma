#include "raylib.h"
#include "bug_move.h"
#include "encre.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define FONT_SIZE 30

typedef enum {
    BUG_GLITCH,
    BUG_INK,
    BUG_COUNT
} BugType;

int main(void) {
    InitWindow(1920, 1000, "Bug de texte");
    SetTargetFPS(60);

    Font font = LoadFontEx("assets/LiberationMono.ttf", FONT_SIZE, NULL, 0);
    if (!font.texture.id) {
        printf("Erreur : impossible de charger la police.\n");
        return 1;
    }

    LoadTextFile("assets/text.txt");
    srand(time(NULL));

    float scrollOffset = 0;
    float scrollSpeed = 10;

    float BugInterval = 1.0f;
    float BugTimer = 0.0f;

    BugType currentBug = BUG_GLITCH;

    InitInkBug(); // Initialise l'encre au cas où

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        if (IsKeyPressed(KEY_SPACE)) {
            currentBug = (currentBug + 1) % BUG_COUNT;
            if (currentBug == BUG_INK) {
                InitInkBug();
            }
        }

        scrollOffset -= scrollSpeed * delta;
        if (scrollOffset < -lineCount * LINE_HEIGHT - 100)
            scrollOffset = GetScreenHeight();

        switch (currentBug) {
            case BUG_GLITCH:
                BugTimer += delta;
                if (BugTimer >= BugInterval) {
                    ChooseWordsToMove(20);
                    BugTimer = 0.0f;
                }
                UpdateMovedWords(delta);
                break;

            case BUG_INK:
                UpdateInkBug(delta);
                break;

            case BUG_COUNT:
                // rien à faire
                break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentBug) {
            case BUG_GLITCH:
                DrawTextWithBug(font, delta, scrollOffset);
                break;

            case BUG_INK:
                DrawTextWithInk(font, scrollOffset);
                break;

            case BUG_COUNT:
                break;
        }

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}

