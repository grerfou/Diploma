#include "raylib.h"
#include "bug_move.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(void) {
    InitWindow(1920, 1000, "Bug de texte");
    SetTargetFPS(60);

    Font font = LoadFont("assets/SpaceMono-Regular.ttf");
    if (!font.texture.id) {
        printf("Erreur : impossible de charger la police.\n");
        return 1;
    }

    LoadTextFile("assets/text.txt");
    srand(time(NULL));

    float scrollOffset = 0;
    float scrollSpeed = 10;  // pixels/sec

    float BugInterval = 1.0f;
    float BugTimer = 0.0f;

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();
        scrollOffset -= scrollSpeed * delta;
        if (scrollOffset < -lineCount * LINE_HEIGHT - 100) scrollOffset = GetScreenHeight();

        UpdateMovedWords(delta);
        BugTimer += delta;
        if (BugTimer >= BugInterval) {
            ChooseWordsToMove(20);
            BugTimer = 0.0f;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawTextWithBug(font, delta, scrollOffset);

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
    return 0;
}

