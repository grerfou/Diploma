#include <raylib.h>
#include <stdio.h>
#include "argile.h"

int main(void)
{

    //int width = 3840;
    int width = 1920;
    //int height = 2160;
    int height = 1080;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(width, height, "Texte animé avec alpha");

    SetWindowPosition(1920, 1);
    
    // Passer en fullscreen fenêtré (bordeless window)
    SetWindowState(FLAG_WINDOW_UNDECORATED);
    SetWindowSize(width, height);

    // Tu peux modifier ces paramètres ici :
    argileMaxLength = 400;
    argileTextAngle = 3.0f;
    argileTextWidth = 300;
    argileTextPos = (Vector2){900, 350};
    argileFontSize = 9;
    argileLineSpacing = 6;
     
    if (!Argile_Init("./assets/text.txt")) {
        CloseWindow();
        printf("Erreur : impossible de charger text.txt\n");
        return 1;
    }

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        Argile_Draw();
        EndDrawing();
    }

    Argile_Close();
    CloseWindow();

    return 0;
}

