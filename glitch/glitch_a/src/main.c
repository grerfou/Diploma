#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"

#include "text_loader.h"
#include "bug_move.h"
#include "bug_letters.h"

typedef enum {
    BUG_MOVE,
    BUG_LETTERS,
    BUG_COUNT
} BugType;

int main(void) {
    InitWindow(1920, 1800, "Titan Sim - Change Bug with Space");

    // Chargement de la police et texte
    Font font = LoadFontEx("assets/LiberationMono.ttf", 18, NULL, 8200);
    LoadTextFromFile("assets/text.txt");

    float scrollOffset = 0.0f;
    float deltaTime = 0.01f;

    // Initialisation bugs
    InitGlitchBug();
    InitLettersBug();

    BugType currentBug = BUG_MOVE;
    bool spacePressedLastFrame = false;

    // Choisir les mots à bugger selon le bug actif
    ChooseWordToMove(30);     // Exemple : 30% de mots buggués pour move
    ChooseWordsToScramble(50); // Exemple : 20% de mots buggués pour letters

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        deltaTime = GetFrameTime();

        // Gestion scroll texte simple
        if (IsKeyDown(KEY_DOWN)) scrollOffset -= 100 * deltaTime;
        if (IsKeyDown(KEY_UP)) scrollOffset += 100 * deltaTime;

        // Passage au bug suivant à la touche espace (pas de répétition si maintenue)
        bool spacePressed = IsKeyPressed(KEY_SPACE);
        if (spacePressed && !spacePressedLastFrame) {
            currentBug = (currentBug + 1) % BUG_COUNT;
        }
        spacePressedLastFrame = spacePressed;

        // Mise à jour selon bug actif
        switch (currentBug) {
            case BUG_MOVE:
                UpdateGlitchBug(deltaTime);
                //DrawGlitchBug(font, scrollOffset);
                break;
            case BUG_LETTERS:
                UpdateLettersBug(deltaTime);
                break;
            default:
                break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        // Dessiner selon bug actif
        switch (currentBug) {
            case BUG_MOVE:
                DrawGlitchBug(font, scrollOffset);
                break;
            case BUG_LETTERS:
                DrawLettersBug(font, scrollOffset);
                break;
            default:
                break;
        }

        // Afficher info
        DrawText("Press SPACE to switch bugs", 10, 10, 20, LIGHTGRAY);
        DrawText(currentBug == BUG_MOVE ? "Current Bug: MOVE" : "Current Bug: LETTERS", 10, 40, 20, YELLOW);

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();

    return 0;
}

