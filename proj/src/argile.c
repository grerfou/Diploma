#include "argile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include <rlgl.h>
#include <ctype.h>

// === Paramètres modifiables ===
int argileMaxLength = 1000;
float argileTextAngle = 0.0f;
int argileTextWidth = 400;
Vector2 argileTextPos = { 100, 100 };
int argileFontSize = 20;
int argileLineSpacing = 5;
float argileFadeSpeed = 1.0f;

// === Internes ===
static char *text = NULL;
static float alpha = 0.0f;
static int fadeDirection = 1;
static Font customFont;        // <<< La police personnalisée
static bool fontLoaded = false;

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

    // === Charger la police personnalisée ===
    if (FileExists("assets/LiberationMono.ttf")) {
        customFont = LoadFontEx("assets/LiberationMono.ttf", 64, 0, 0);
        SetTextureFilter(customFont.texture, TEXTURE_FILTER_POINT);
        fontLoaded = true;
    } else {
        customFont = GetFontDefault();
    }

    return true;
}

void DrawTextWrapped(Font font, const char *str, int maxLen, Vector2 pos, int maxWidth, int fontSize, int spacing, Color tint)
{
    int x = 0, y = 0;
    const char *ptr = str;
    int lineHeight = fontSize + spacing;
    int drawn = 0;

    rlPushMatrix();
    rlTranslatef(pos.x, pos.y, 0);
    rlRotatef(argileTextAngle, 0, 0, 1);

    while (*ptr && drawn < maxLen)
    {
        while (*ptr && isspace(*ptr) && *ptr != '\n') ptr++;

        if (*ptr == '\n') {
            x = 0;
            y += lineHeight;
            ptr++;
            continue;
        }

        const char *start = ptr;
        while (*ptr && !isspace(*ptr) && *ptr != '\n') ptr++;

        int wordLen = ptr - start;
        if (drawn + wordLen > maxLen) break;

        char word[256] = {0};
        if (wordLen > 255) wordLen = 255;
        strncpy(word, start, wordLen);

        int wordWidth = MeasureTextEx(font, word, fontSize, 1).x;

        if (x + wordWidth > maxWidth) {
            x = 0;
            y += lineHeight;
        }

        DrawTextEx(font, word, (Vector2){(float)x, (float)y}, fontSize, 2, tint);
        x += wordWidth;
        drawn += wordLen;
    }

    rlPopMatrix();
}

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
    DrawTextWrapped(customFont, text, argileMaxLength, argileTextPos, argileTextWidth, argileFontSize, argileLineSpacing, textColor);
}

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

