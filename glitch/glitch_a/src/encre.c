#include "encre.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_INK_WORDS 100
#define LINE_HEIGHT 40
#define FONT_SIZE 30
#define TEXT_MARGIN_LEFT 500
#define TEXT_MARGIN_TOP 0
#define WORD_SPACING 10

Color COLOR_INK = {50, 50, 150, 150}; // encre bleu foncé
Color COLOR_TEXTE = {255, 255, 255, 255}; // texte normal

typedef struct {
    int lineIndex;
    int wordIndexInLine;
    Vector2 position;
    float timer;
    float bleedTime;
    bool active;
} InkWord;

InkWord inkWords[MAX_INK_WORDS];
int inkWordCount = 0;
extern char *lines[];
extern int lineCount;

void InitInkBug() {
    inkWordCount = 0;
    for (int i = 0; i < MAX_INK_WORDS; i++) {
        inkWords[i].active = false;
    }
}

void ChooseWordsToInk(int percentage) {
    inkWordCount = 0;

    typedef struct {
        int lineIndex;
        int wordIndexInLine;
        char word[64];
        Vector2 position;
    } WordInfo;

    WordInfo allWords[2048];
    int totalWords = 0;

    for (int i = 0; i < lineCount; i++) {
        char lineCopy[1024];
        strcpy(lineCopy, lines[i]);

        char *token = strtok(lineCopy, " \t\n");
        float x = TEXT_MARGIN_LEFT;
        int wordIndex = 0;

        while (token && totalWords < 2048) {
            strcpy(allWords[totalWords].word, token);
            allWords[totalWords].lineIndex = i;
            allWords[totalWords].wordIndexInLine = wordIndex;
            allWords[totalWords].position = (Vector2){x, TEXT_MARGIN_TOP + i * LINE_HEIGHT};

            x += MeasureText(token, FONT_SIZE) + WORD_SPACING;
            token = strtok(NULL, " \t\n.,;!?");
            wordIndex++;
            totalWords++;
        }
    }

    int toBleed = (percentage * totalWords) / 100;
    for (int i = 0; i < toBleed && inkWordCount < MAX_INK_WORDS; i++) {
        int index = rand() % totalWords;
        InkWord *w = &inkWords[inkWordCount++];
        w->lineIndex = allWords[index].lineIndex;
        w->wordIndexInLine = allWords[index].wordIndexInLine;
        w->position = allWords[index].position;
        w->timer = 0;
        w->bleedTime = ((rand() % 100) / 100.0f) * 0.5f + 0.2f; // random entre 0.2 et 0.7 sec
        w->active = true;
    }
}

void UpdateInkBug(float delta) {
    for (int i = 0; i < inkWordCount; i++) {
        inkWords[i].timer += delta;
        if (inkWords[i].timer > inkWords[i].bleedTime) {
            inkWords[i].active = false;
        }
    }
    ChooseWordsToInk(20); // régénère régulièrement
}

void DrawTextWithInk(Font font, float scrollOffset) {
    for (int i = 0; i < lineCount; i++) {
        char *line = lines[i];
        char *tmp = strdup(line);
        char *tokens[64] = {0};
        int tokenCount = 0;

        char *word = strtok(tmp, " \n");
        while (word && tokenCount < 64) {
            tokens[tokenCount++] = strdup(word);
            word = strtok(NULL, " \n");
        }

        float xPos = TEXT_MARGIN_LEFT;
        float yPos = TEXT_MARGIN_TOP + i * LINE_HEIGHT + scrollOffset;

        for (int j = 0; j < tokenCount; j++) {
            bool inked = false;

            for (int k = 0; k < inkWordCount; k++) {
                if (inkWords[k].active &&
                    inkWords[k].lineIndex == i &&
                    inkWords[k].wordIndexInLine == j) {

                    Vector2 pos = inkWords[k].position;
                    pos.y += scrollOffset;

                    DrawTextEx(font, tokens[j], pos, FONT_SIZE, 2, COLOR_INK);
                    inked = true;
                    break;
                }
            }

            if (!inked) {
                DrawTextEx(font, tokens[j], (Vector2){xPos, yPos}, FONT_SIZE, 2, COLOR_TEXTE);
            }

            xPos += MeasureTextEx(font, tokens[j], FONT_SIZE, 2).x + WORD_SPACING;
            free(tokens[j]);
        }

        free(tmp);
    }
}

