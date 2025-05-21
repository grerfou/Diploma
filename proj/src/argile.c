#include "argile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <rlgl.h>
#include <ctype.h>

// === Paramètres modifiables ===
int argileMaxLength = 1000;
Vector2 argileTextPos = { 100, 100 };
int argileFontSize = 7;
int argileLineSpacing = 0;
float argileFadeSpeed = 1.0f;
float argileTextWidth = 160.0f;  // Largeur du texte (modifiable dynamiquement)
//float argileTextWidth = 260.0f;  // Largeur du texte (modifiable dynamiquement)


// Pour définir les 4 points du plan (modifiable via sliders)
Vector2 argileTopLeft = {100, 100};
Vector2 argileTopRight = {500, 100};
Vector2 argileBottomLeft = {100, 300};
Vector2 argileBottomRight = {500, 300};

// === Internes ===
static char *text = NULL;
static float alpha = 0.0f;
static int fadeDirection = 1;
static Font customFont;        // La police personnalisée
static bool fontLoaded = false;

// Gestion sliders
static bool showSliders = true;
static int activeSlider = -1;
static const float sliderRadius = 8.0f;

// Fonction utilitaire
static bool IsMouseOverPoint(Vector2 point, Vector2 mousePos, float radius) {
    float dx = point.x - mousePos.x;
    float dy = point.y - mousePos.y;
    return (dx*dx + dy*dy) <= (radius * radius);
}

// Gestion sliders (dessin + interaction)
static void DrawAndHandleSliders(void)
{
    Vector2 mousePos = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        if (IsMouseOverPoint(argileTopLeft, mousePos, sliderRadius)) activeSlider = 0;
        else if (IsMouseOverPoint(argileTopRight, mousePos, sliderRadius)) activeSlider = 1;
        else if (IsMouseOverPoint(argileBottomLeft, mousePos, sliderRadius)) activeSlider = 2;
        else if (IsMouseOverPoint(argileBottomRight, mousePos, sliderRadius)) activeSlider = 3;
        else activeSlider = -1;
    }
    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        activeSlider = -1;
    }

    if (activeSlider != -1)
    {
        switch(activeSlider)
        {
            case 0: argileTopLeft = mousePos; break;
            case 1: argileTopRight = mousePos; break;
            case 2: argileBottomLeft = mousePos; break;
            case 3: argileBottomRight = mousePos; break;
        }
    }

    if (showSliders)
    {
        DrawCircleV(argileTopLeft, sliderRadius, RED);
        DrawCircleV(argileTopRight, sliderRadius, GREEN);
        DrawCircleV(argileBottomLeft, sliderRadius, BLUE);
        DrawCircleV(argileBottomRight, sliderRadius, YELLOW);
    }
}

/*
// Dessiner le texte déformé sur le plan défini par 4 points
static void DrawTextOnPlane(Font font, const char *str, int maxLen, int fontSize, int spacing, Color tint)
{
    int lineHeight = fontSize + spacing;
    int drawn = 0;

    int lineCount = 0;
    int maxLines = maxLen / 20 + 1; // Estimation max lignes (ajuster si besoin)

    while (str[drawn] && lineCount < maxLines)
    {
        // Construire la ligne jusqu’au saut de ligne ou max 20 chars
        char line[256] = {0};
        int i = 0;
        while (str[drawn] && str[drawn] != '\n' && i < 20 && drawn < maxLen)
        {
            line[i++] = str[drawn++];
        }
        if (str[drawn] == '\n') drawn++;
        line[i] = '\0';

        float tY = (float)lineCount / (float)maxLines;

        Vector2 start = Vector2Lerp(argileTopLeft, argileBottomLeft, tY);
        Vector2 end   = Vector2Lerp(argileTopRight, argileBottomRight, tY);

        int lineWidthPx = MeasureTextEx(font, line, fontSize, 10).x;

        // Dessiner chaque caractère avec interpolation sur la ligne
        float xStart = start.x;
        float yStart = start.y;
        float xEnd = end.x;
        float yEnd = end.y;

        float lineLen = sqrtf((xEnd - xStart)*(xEnd - xStart) + (yEnd - yStart)*(yEnd - yStart));

        float cursorX = 0;
        for (int c = 0; c < i; c++)
        {
            char ch[2] = { line[c], 0 };
            int charWidth = (int)MeasureTextEx(font, ch, fontSize, 1).x;

            float tChar = cursorX / lineLen;
            Vector2 pos = {
                xStart + (xEnd - xStart) * tChar,
                yStart + (yEnd - yStart) * tChar
            };

            DrawTextEx(font, ch, pos, fontSize, 1, tint);

            cursorX += (float)charWidth;
        }

        lineCount++;
    }
}
*/

static void DrawTextOnPlane(Font font, const char *str, int maxLen, int fontSize, int spacing, Color tint)
{
    int lineHeight = fontSize + spacing;
    int totalDrawn = 0;
    int index = 0;

    float heightLeft = Vector2Distance(argileTopLeft, argileBottomLeft);
    float heightRight = Vector2Distance(argileTopRight, argileBottomRight);
    float planHeight = (heightLeft + heightRight) / 2.0f;

    int maxLines = (int)(planHeight / (float)lineHeight);
    if (maxLines < 1) maxLines = 1;

    for (int lineCount = 0; lineCount < maxLines && str[index] && totalDrawn < maxLen; lineCount++)
    {
        char line[512] = {0};
        float lineWidth = 0;
        int lineCharCount = 0;
        int lineStart = index;

        // Lire les caractères dans la ligne tant qu’on ne dépasse pas argileTextWidth ni argileMaxLength
        while (str[index] && str[index] != '\n')
        {
            if (totalDrawn >= maxLen) break;

            char ch[2] = { str[index], 0 };
            float chWidth = MeasureTextEx(font, ch, fontSize, 1).x;

            if (lineWidth + chWidth > argileTextWidth) break;

            line[lineCharCount++] = str[index++];
            lineWidth += chWidth;
            totalDrawn++;
        }

        line[lineCharCount] = '\0';

        // Saut de ligne explicite dans le texte
        if (str[index] == '\n') {
            index++;
        }

        // Position verticale sur le plan
        float tY = ((float)lineCount + 0.5f) / (float)maxLines;
        Vector2 start = Vector2Lerp(argileTopLeft, argileBottomLeft, tY);
        Vector2 end   = Vector2Lerp(argileTopRight, argileBottomRight, tY);

        float cursor = 0;
        Vector2 dir = Vector2Normalize(Vector2Subtract(end, start));

        for (int c = 0; c < lineCharCount; c++)
        {
            char ch[2] = { line[c], 0 };
            float chWidth = MeasureTextEx(font, ch, fontSize, 1).x;

            Vector2 pos = Vector2Add(start, Vector2Scale(dir, cursor));
            DrawTextEx(font, ch, pos, fontSize, 1, tint);
            cursor += chWidth;
        }
    }
}





bool Argile_Init(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f) return false;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    text = (char *)malloc(size + 1);
    if (!text) {
        fclose(f);
        return false;
    }

    fread(text, 1, size, f);
    text[size] = '\0';
    fclose(f);

    if (FileExists("assets/LiberationMono.ttf")) {
        customFont = LoadFont("assets/LiberationMono.ttf");
        fontLoaded = true;
    } else {
        customFont = GetFontDefault();
    }

    return true;
}


/*
void Argile_Draw(void)
{
    ClearBackground(BLACK);

    if (!text) return;

    float delta = GetFrameTime();
    alpha += fadeDirection * argileFadeSpeed * delta;

    if (alpha >= 1.0f) {
        alpha = 1.0f;
        fadeDirection = -1;
    } else if (alpha <= 0.0f) {
        alpha = 0.0f;
        fadeDirection = 1;
    }

    Color textColor = (Color){255, 255, 255, (unsigned char)(alpha * 255)};

    // === Mise à jour automatique des coins droits selon argileTextWidth ===
    Vector2 dirTop = Vector2Normalize((Vector2){
        argileTopRight.x - argileTopLeft.x,
        argileTopRight.y - argileTopLeft.y
    });
    Vector2 dirBottom = Vector2Normalize((Vector2){
        argileBottomRight.x - argileBottomLeft.x,
        argileBottomRight.y - argileBottomLeft.y
    });

    argileTopRight = (Vector2){
        argileTopLeft.x + dirTop.x * argileTextWidth,
        argileTopLeft.y + dirTop.y * argileTextWidth
    };

    argileBottomRight = (Vector2){
        argileBottomLeft.x + dirBottom.x * argileTextWidth,
        argileBottomLeft.y + dirBottom.y * argileTextWidth
    };

    // === Affichage texte ===
    DrawTextOnPlane(customFont, text, argileMaxLength, argileFontSize, argileLineSpacing, textColor);

    // Touche TAB pour afficher/masquer sliders
    if (IsKeyPressed(KEY_TAB)) {
        showSliders = !showSliders;
    }

    DrawAndHandleSliders();
}

*/


void Argile_Draw(void)
{
    ClearBackground(BLACK);

    if (!text) return;

    float delta = GetFrameTime();
    alpha += fadeDirection * argileFadeSpeed * delta;

    if (alpha >= 1.0f) {
        alpha = 1.0f;
        fadeDirection = -1;
    } else if (alpha <= 0.0f) {
        alpha = 0.0f;
        fadeDirection = 1;
    }

    // === Changement largeur via touche W ===
    if (IsKeyPressed(KEY_W)) {
        if (argileTextWidth == 160.0f)
            argileTextWidth = 260.0f;
        else
            argileTextWidth = 160.0f;
    }

    Color textColor = (Color){255, 255, 255, (unsigned char)(alpha * 255)};

    // === Mise à jour auto des coins droits selon argileTextWidth ===
    if (activeSlider == -1) {
        Vector2 dirTop = Vector2Normalize(Vector2Subtract(argileTopRight, argileTopLeft));
        Vector2 dirBottom = Vector2Normalize(Vector2Subtract(argileBottomRight, argileBottomLeft));

        argileTopRight = Vector2Add(argileTopLeft, Vector2Scale(dirTop, argileTextWidth));
        argileBottomRight = Vector2Add(argileBottomLeft, Vector2Scale(dirBottom, argileTextWidth));
    }

    // === Affichage texte déformé ===
    DrawTextOnPlane(customFont, text, argileMaxLength, argileFontSize, argileLineSpacing, textColor);

    // === Affichage debug ===
    DrawText(TextFormat("Width: %.0f (Press W to toggle)", argileTextWidth), 10, 10, 20, GRAY);

    // Toggle sliders avec TAB
    if (IsKeyPressed(KEY_TAB)) {
        showSliders = !showSliders;
    }

    DrawAndHandleSliders();
}























/*

void Argile_Draw(void)
{
    ClearBackground(BLACK);

    if (!text) return;

    float delta = GetFrameTime();
    alpha += fadeDirection * argileFadeSpeed * delta;

    if (alpha >= 1.0f) {
        alpha = 1.0f;
        fadeDirection = -1;
    } else if (alpha <= 0.0f) {
        alpha = 0.0f;
        fadeDirection = 1;
    }

    Color textColor = (Color){255, 255, 255, (unsigned char)(alpha * 255)};

    DrawTextOnPlane(customFont, text, argileMaxLength, argileFontSize, argileLineSpacing, textColor);

    if (IsKeyPressed(KEY_TAB)) {
        showSliders = !showSliders;
    }

    DrawAndHandleSliders();
}

*/

void Argile_Close(void)
{
    if (text) {
        free(text);
        text = NULL;
    }

    if (fontLoaded) {
        UnloadFont(customFont);
    }
}
