#include "raylib.h"
#include "text_loader.h"
#include "bug_move.h"
#include "bug_stub.h"
#include "vhs_glitch.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define FONT_SIZE 30

typedef enum {
    BUG_GLITCH,
    BUG_STUB, // à remplacer plus tard
    VHS_GLITCH,
    BUG_COUNT
} BugType;

int main(void) {
    InitWindow(1920, 1000, "Bug de texte");
    SetTargetFPS(60);

    Font font = LoadFontEx("assets/LiberationMono.ttf", FONT_SIZE, NULL, 8200);
    if (!font.texture.id) {
        printf("Erreur : impossible de charger la police.\n");
        return 1;
    }

    LoadTextFromFile("assets/text.txt");
    srand(time(NULL));

    float scrollOffset = 0;
    float scrollSpeed = 10;

    BugType currentBug = BUG_GLITCH;
    float bugTimer = 0.0f;
    float bugInterval = 1.0f;

    InitGlitchBug();
    InitStubBug(); 
    InitVHSGlitch();

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        if (IsKeyPressed(KEY_SPACE)) {
            currentBug = (currentBug + 1) % BUG_COUNT;
        }

        scrollOffset -= scrollSpeed * delta;
        if (scrollOffset < -lineCount * LINE_HEIGHT - 100)
            scrollOffset = GetScreenHeight();

        switch (currentBug) {
            case BUG_GLITCH:
                bugTimer += delta;
                if (bugTimer >= bugInterval) {
                    //ChooseWordsToMove(20);
                    ChooseLettersToMove(20);
                    bugTimer = 0.0f;
                }
                UpdateGlitchBug(delta);
                break;

            case VHS_GLITCH: 
                UpdateVHSGlitch(delta);
                break;

            case BUG_STUB:
                UpdateStubBug(delta);
                break;

            default: break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        switch (currentBug) {
            case BUG_GLITCH:
                NoteGlitch(font, scrollOffset);
                DrawGlitchBug(font, scrollOffset);
                break;
            
            case VHS_GLITCH:
                NoteVHS (font, scrollOffset);
                DrawVHSGlitch(font, delta, scrollOffset);
                break;

            case BUG_STUB:
                NoteStub(font, scrollOffset);
                DrawStubBug(font, delta, scrollOffset);
                break;

            default: break;
        }

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}

