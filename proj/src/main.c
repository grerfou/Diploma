#include <stdio.h>
#include <stdbool.h>
#include "raylib.h"

#include "argile.h"
#include "paper.h"

typedef enum {
    MODE_ARGILE,
    MODE_PAPER,
    MODE_COUNT
} AppMode;

int main(void) {
    const int width = 1920;
    const int height = 1080;

    InitWindow(width, height, "Switch Program with Space");

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    SetWindowPosition(1920, 0);
    SetWindowState(FLAG_WINDOW_UNDECORATED);
    SetWindowSize(width, height);

    AppMode currentMode = MODE_ARGILE;
    bool argileLoaded = false;
    bool paperLoaded = false;

    // Initialise le premier mode (ARGILE)
    if (!Argile_Init("./assets/text.txt")) {
        CloseWindow();
        printf("Erreur : impossible de charger text.txt\n");
        return 1;
    }
    argileLoaded = true;

    bool spacePressedLastFrame = false;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Detecte appui barre espace (une fois)
        bool spacePressed = IsKeyPressed(KEY_SPACE);

        if (spacePressed && !spacePressedLastFrame) {
            // Switch de mode
            switch (currentMode) {
                case MODE_ARGILE:
                    // Ferme argile
                    if (argileLoaded) {
                        Argile_Close();
                        argileLoaded = false;
                    }
                    // Ouvre paper
                    if (!Paper_Init("./assets/text.txt")) {
                        CloseWindow();
                        printf("Erreur : impossible de charger text.txt\n");
                        return 1;
                    }
                    paperLoaded = true;
                    currentMode = MODE_PAPER;
                    break;

                case MODE_PAPER:
                    // Ferme paper
                    if (paperLoaded) {
                        Paper_Close();
                        paperLoaded = false;
                    }
                    // Ouvre argile
                    if (!Argile_Init("./assets/text.txt")) {
                        CloseWindow();
                        printf("Erreur : impossible de charger text.txt\n");
                        return 1;
                    }
                    argileLoaded = true;
                    currentMode = MODE_ARGILE;
                    break;

                default:
                    currentMode = MODE_ARGILE;
                    break;
            }
        }
        spacePressedLastFrame = spacePressed;

        // Dessin
        BeginDrawing();
        ClearBackground(RAYWHITE);

        switch (currentMode) {
            case MODE_ARGILE:
                if (argileLoaded) Argile_Draw();
                break;

            case MODE_PAPER:
                if (paperLoaded) Paper_Draw();
                break;

            default:
                DrawText("Mode inconnu", 100, 100, 30, RED);
                break;
        }

        DrawText("Press SPACE to switch mode", 10, 10, 20, DARKGRAY);
        DrawText(currentMode == MODE_ARGILE ? "Current Mode: ARGILE" : "Current Mode: PAPER", 10, 40, 20, DARKGREEN);

        EndDrawing();
    }

    // Nettoyage
    if (argileLoaded) Argile_Close();
    if (paperLoaded) Paper_Close();

    CloseWindow();
    return 0;
}

