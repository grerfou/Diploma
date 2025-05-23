// paper.c
#include "paper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include <raymath.h>

// === Paramètres modifiables ===
int paperFontSize = 10;
int paperLineSpacing = 2;
float paperTextWidth = 250.0f;

// Coins du plan (modifiable par sliders)
Vector2 paperTopLeft = {100, 100};
Vector2 paperTopRight = {500, 100};
Vector2 paperBottomLeft = {100, 300};
Vector2 paperBottomRight = {500, 300};

// === Variables internes ===
static char *paperText = NULL;
static Font handwritingFont;
static bool fontLoaded = false;

// Animation écriture lettre par lettre
static int lettersDisplayed = 0;
static float letterTimer = 0.0f;
static const float LETTER_DELAY = 0.05f;

// Sliders
static bool showSliders = true;
static int activeSlider = -1;
static const float sliderRadius = 8.0f;

// Défilement vertical du texte
static float verticalScrollOffset = 0.0f;

// ==== Fonctions ====

static bool IsMouseOverPoint(Vector2 point, Vector2 mousePos, float radius) {
    return CheckCollisionPointCircle(mousePos, point, radius);
}

static void DrawAndHandleSliders(void) {
    Vector2 mousePos = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (IsMouseOverPoint(paperTopLeft, mousePos, sliderRadius)) activeSlider = 0;
        else if (IsMouseOverPoint(paperTopRight, mousePos, sliderRadius)) activeSlider = 1;
        else if (IsMouseOverPoint(paperBottomLeft, mousePos, sliderRadius)) activeSlider = 2;
        else if (IsMouseOverPoint(paperBottomRight, mousePos, sliderRadius)) activeSlider = 3;
        else activeSlider = -1;
    }
    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        activeSlider = -1;
    }

    if (activeSlider != -1) {
        Vector2 mousePos = GetMousePosition();
        switch (activeSlider) {
            case 0: paperTopLeft = mousePos; break;
            case 1: paperTopRight = mousePos; break;
            case 2: paperBottomLeft = mousePos; break;
            case 3: paperBottomRight = mousePos; break;
        }
    }

    if (showSliders) {
        DrawCircleV(paperTopLeft, sliderRadius, RED);
        DrawCircleV(paperTopRight, sliderRadius, GREEN);
        DrawCircleV(paperBottomLeft, sliderRadius, BLUE);
        DrawCircleV(paperBottomRight, sliderRadius, YELLOW);
    }
}

static void DrawHandwritingTextOnPlaneScrolling(Font font, const char *str, int maxLetters, int fontSize, int spacing, Color baseColor)
{
    int lineHeight = fontSize + spacing;
    int totalDrawn = 0;
    int index = 0;

    // Découpage en lignes
    char lines[1024][512];
    int numLines = 0;

    while (str[index] && totalDrawn < maxLetters && numLines < 1024)
    {
        char line[512] = {0};
        float lineWidth = 0;
        int lineCharCount = 0;

        while (str[index] && str[index] != '\n' && totalDrawn < maxLetters)
        {
            char ch[2] = { str[index], 0 };
            float chWidth = MeasureTextEx(font, ch, fontSize, 1).x;

            if (lineWidth + chWidth > paperTextWidth) break;

            line[lineCharCount++] = str[index++];
            lineWidth += chWidth;
            totalDrawn++;
        }

        line[lineCharCount] = '\0';
        strcpy(lines[numLines++], line);

        if (str[index] == '\n') index++;
    }

    int totalTextHeight = lineHeight * numLines;

    float planeHeight = Vector2Distance(paperTopLeft, paperBottomLeft);

    // Scroll vers le haut si texte trop grand
    if (totalTextHeight > planeHeight) {
        verticalScrollOffset += (lineHeight * GetFrameTime() * 20); // ajuster vitesse scroll ici
        if (verticalScrollOffset > (totalTextHeight - planeHeight))
            verticalScrollOffset = totalTextHeight - planeHeight;
    }
    else {
        verticalScrollOffset = 0;
    }

    int letterIndex = 0;

    for (int i = 0; i < numLines; i++)
    {
        float tY = ((lineHeight * i) - verticalScrollOffset) / planeHeight;

        if (tY < 0 || tY > 1) continue;

        Vector2 start = Vector2Lerp(paperTopLeft, paperBottomLeft, tY);
        Vector2 end = Vector2Lerp(paperTopRight, paperBottomRight, tY);

        float cursor = 0;
        Vector2 dir = Vector2Normalize(Vector2Subtract(end, start));

        for (int c = 0; lines[i][c]; c++)
        {
            if (letterIndex >= maxLetters) return;

            char ch[2] = { lines[i][c], 0 };
            float chWidth = MeasureTextEx(font, ch, fontSize, 1).x;

            Vector2 pos = Vector2Add(start, Vector2Scale(dir, cursor));

            DrawTextEx(font, ch, pos, fontSize, 1, baseColor);

            cursor += chWidth;
            letterIndex++;
        }
    }
}

// Init depuis fichier texte et police
bool Paper_Init(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f) return false;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    paperText = (char *)malloc(size + 1);
    if (!paperText) {
        fclose(f);
        return false;
    }

    fread(paperText, 1, size, f);
    paperText[size] = '\0';
    fclose(f);

    if (FileExists("assets/LiberationMono.ttf")) {
        handwritingFont = LoadFont("assets/LiberationMono.ttf");
        fontLoaded = true;
    } else {
        handwritingFont = GetFontDefault();
    }

    lettersDisplayed = 0;
    letterTimer = 0.0f;
    verticalScrollOffset = 0.0f;

    return true;
}

// Dessin principal
void Paper_Draw(void)
{
    ClearBackground(BLACK);
    if (!paperText) return;

    letterTimer += GetFrameTime();

    if (lettersDisplayed < (int)strlen(paperText) && letterTimer >= LETTER_DELAY) {
        letterTimer = 0.0f;
        lettersDisplayed++;
    }

    Color textColor = (Color){ 255, 255, 255, 255 };

    DrawHandwritingTextOnPlaneScrolling(handwritingFont, paperText, lettersDisplayed, paperFontSize, paperLineSpacing, textColor);

    if (IsKeyPressed(KEY_TAB)) showSliders = !showSliders;
    DrawAndHandleSliders();

    DrawText("Press TAB to toggle sliders", 10, 10, 20, GRAY);
}

// Nettoyage
void Paper_Close(void)
{
    if (paperText) {
        free(paperText);
        paperText = NULL;
    }
    if (fontLoaded) {
        UnloadFont(handwritingFont);
    }
}
