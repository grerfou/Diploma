#include "paper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include <raymath.h>

// === Paramètres modifiables ===
static int paperMaxLetters = 10; 
static int paperFontSize = 20;
static int paperLineSpacing = 0;
static float paperTextWidth = 370.0f;

// Coins du plan (modifiable avec sliders)
static Vector2 paperTopLeft = {100, 100};
static Vector2 paperTopRight = {500, 100};
static Vector2 paperBottomLeft = {100, 300};
static Vector2 paperBottomRight = {500, 300};

// === Variables internes ===
static char *paperText = NULL;
static Font handwritingFont;
static bool fontLoaded = false;

// Animation lettre par lettre
static int lettersDisplayed = 0;
static float letterTimer = 0.0f;
static const float LETTER_DELAY = 0.05f; // délai entre chaque lettre (s)

// Sliders
static bool showSliders = true;
static int activeSlider = -1;
static const float sliderRadius = 8.0f;

// === Fonctions internes ===

// Vérifie si la souris est sur un point donné (pour sliders)
static bool IsMouseOverPoint(Vector2 point, Vector2 mousePos, float radius) {
    return CheckCollisionPointCircle(mousePos, point, radius);
}

// Gère le dessin et interaction des sliders sur les coins du plan
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

// Dessine le texte sur le plan, lettre par lettre, sans effets alpha/zoom
static void DrawHandwritingTextOnPlane(Font font, const char *str, int maxLetters, int fontSize, int spacing, Color baseColor)
{
    int lineHeight = fontSize + spacing;
    int totalDrawn = 0;
    int index = 0;

    char lines[1024][512];
    int numLines = 0;

    // Découpe le texte en lignes en fonction de la largeur du plan et maxLetters
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

    float totalHeight = lineHeight * numLines;
    int letterIndex = 0;

    for (int i = 0; i < numLines; i++)
    {
        float tY = (lineHeight * i) / totalHeight;
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

// === Fonctions publiques ===

bool Paper_Init(const char *filename) {
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

    if (FileExists("assets/Satisfy-Regular.ttf")) {
        handwritingFont = LoadFont("assets/Satisfy-Regular.ttf");
        fontLoaded = true;
    } else {
        handwritingFont = GetFontDefault();
    }

    // Reset animation variables
    lettersDisplayed = 0;
    letterTimer = 0.0f;

    return true;
}

void Paper_Draw(void) {
    ClearBackground(BLACK);
    if (!paperText) return;

    float deltaTime = GetFrameTime();
    letterTimer += deltaTime;

    if (lettersDisplayed < (int)strlen(paperText) && letterTimer >= LETTER_DELAY) {
        letterTimer = 0.0f;
        lettersDisplayed++;
    }

    Color textColor = (Color){ 255, 255, 255, 255 };
    DrawHandwritingTextOnPlane(handwritingFont, paperText, lettersDisplayed, paperFontSize, paperLineSpacing, textColor);

    if (IsKeyPressed(KEY_TAB)) showSliders = !showSliders;
    DrawAndHandleSliders();

    DrawText("Press TAB to toggle sliders", 10, 10, 20, GRAY);
}

void Paper_Close(void) {
    if (paperText) {
        free(paperText);
        paperText = NULL;
    }
    if (fontLoaded) {
        UnloadFont(handwritingFont);
    }
}
