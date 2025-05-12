#include "bug_move.h"
#include "text_loader.h"
#include "vhs_glitch.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// === PARAMÈTRES DU BUG ===
#define BUG_MOVE_DURATION 0.001f
#define BUG_MOVE_RANGE_X  2
#define BUG_MOVE_RANGE_Y  2
#define FONT_SIZE         30
#define WORD_SPACING      10
#define TEXT_MARGIN_LEFT  500
#define TEXT_MARGIN_TOP   0

Color COLOR_TEXT = {255, 255, 255, 255};
Color COLOR_BUG = {111, 111, 111, 255};

char *lines[MAX_LINES];
int lineCount = 0;

/*
typedef struct {
    int lineIndex;
    int wordIndexInLine;
    char text[64];
    Vector2 initialPosition;
    Vector2 movePosition;
    bool isMoved;
    float moveTimer;
} WordMoveInfo;
*/

//WordMoveInfo movedWords[MAX_MOVED_WORDS];
//int movedWordsCount = 0;

typedef struct {
    int lineIndex;
    int charIndexInLine;
    char character;
    Vector2 initialPosition;
    Vector2 movePosition;
    bool isMoved;
    float moveTimer;
} LetterMoveInfo;

LetterMoveInfo movedLetters[MAX_MOVED_WORDS];
int movedLettersCount = 0;


/*
void InitGlitchBug(void) {
    for (int i = 0; i < MAX_MOVED_WORDS; i++) {
        movedWords[i].isMoved = false;
        movedWords[i].moveTimer = 0.0f;
        movedWords[i].initialPosition = (Vector2){0, 0};
        movedWords[i].movePosition = (Vector2){0, 0};
    }
}
*/
void InitGlitchBug(void) {
    for (int i = 0; i < MAX_MOVED_WORDS; i++) {
        movedLetters[i].isMoved = false;
        movedLetters[i].moveTimer = 0.0f;
        movedLetters[i].initialPosition = (Vector2){0, 0};
        movedLetters[i].movePosition = (Vector2){0, 0};
    }
}


void NoteGlitch(Font font, float scrollOffset) {
    DrawText("Bug_01 (Move)", 100, 100, 30, RED);
}

/*
void ChooseWordsToMove(int percentage) {
    movedWordsCount = 0;

    typedef struct {
        int lineIndex;
        int wordIndexInLine;
        char word[64];
        Vector2 position;
    } WordInfo;

    WordInfo allWords[2048];
    int totalWords = 0;

    for (int i = 0; i < lineCount; i++) {
        char lineCopy[MAX_LINE_LENGTH];
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

    int wordsToBug = (percentage * totalWords) / 100;

    for (int i = 0; i < wordsToBug && movedWordsCount < MAX_MOVED_WORDS; i++) {
        int index = rand() % totalWords;
        WordInfo w = allWords[index];

        movedWords[movedWordsCount].lineIndex = w.lineIndex;
        movedWords[movedWordsCount].wordIndexInLine = w.wordIndexInLine;
        strcpy(movedWords[movedWordsCount].text, w.word);
        movedWords[movedWordsCount].initialPosition = w.position;
        movedWords[movedWordsCount].moveTimer = 0.0f;
        movedWords[movedWordsCount].isMoved = true;

        float dx = (rand() % (BUG_MOVE_RANGE_X * 2 + 1)) - BUG_MOVE_RANGE_X;
        float dy = (rand() % (BUG_MOVE_RANGE_Y * 2 + 1)) - BUG_MOVE_RANGE_Y;
        movedWords[movedWordsCount].movePosition = (Vector2){
            w.position.x + dx,
            w.position.y + dy
        };

        movedWordsCount++;
    }
}
*/

void ChooseLettersToMove(int percentage) {
    movedLettersCount = 0;

    typedef struct {
        int lineIndex;
        int charIndexInLine;
        char character;
        Vector2 position;
    } LetterInfo;

    LetterInfo allLetters[4096];
    int totalLetters = 0;

    for (int i = 0; i < lineCount; i++) {
        char *line = lines[i];
        float x = TEXT_MARGIN_LEFT;
        float y = TEXT_MARGIN_TOP + i * LINE_HEIGHT;

        int charIndex = 0;
        for (int j = 0; line[j] != '\0'; j++) {
            if (line[j] == '\n') continue;

            if (totalLetters >= 4096) break;

            allLetters[totalLetters].lineIndex = i;
            allLetters[totalLetters].charIndexInLine = charIndex;
            allLetters[totalLetters].character = line[j];
            allLetters[totalLetters].position = (Vector2){x, y};

            Vector2 charSize = MeasureTextEx(GetFontDefault(), "A", FONT_SIZE, 2);
            x += charSize.x;
            charIndex++;
            totalLetters++;
            if (totalLetters >= 4096) break;
        }
    }

    int lettersToBug = (percentage * totalLetters) / 100;

    for (int i = 0; i < lettersToBug && movedLettersCount < MAX_MOVED_WORDS; i++) {
        int index = rand() % totalLetters;
        LetterInfo l = allLetters[index];

        movedLetters[movedLettersCount].lineIndex = l.lineIndex;
        movedLetters[movedLettersCount].charIndexInLine = l.charIndexInLine;
        movedLetters[movedLettersCount].character = l.character;
        movedLetters[movedLettersCount].initialPosition = l.position;
        movedLetters[movedLettersCount].moveTimer = 0.0f;
        movedLetters[movedLettersCount].isMoved = true;

        float dx = (rand() % (BUG_MOVE_RANGE_X * 2 + 1)) - BUG_MOVE_RANGE_X;
        float dy = (rand() % (BUG_MOVE_RANGE_Y * 2 + 1)) - BUG_MOVE_RANGE_Y;
        movedLetters[movedLettersCount].movePosition = (Vector2){
            l.position.x + dx,
            l.position.y + dy
        };

        movedLettersCount++;
    }
}

/*
void UpdateGlitchBug(float deltaTime) {
    for (int i = 0; i < movedWordsCount; i++) {
        if (!movedWords[i].isMoved && movedWords[i].moveTimer == 0.0f) {
            float offsetX = (rand() % (BUG_MOVE_RANGE_X * 2 + 1)) - BUG_MOVE_RANGE_X;
            float offsetY = (rand() % (BUG_MOVE_RANGE_Y * 2 + 1)) - BUG_MOVE_RANGE_Y;
            movedWords[i].movePosition = (Vector2){
                movedWords[i].initialPosition.x + offsetX,
                movedWords[i].initialPosition.y + offsetY
            };
            movedWords[i].isMoved = true;
        }

        if (movedWords[i].isMoved) {
            movedWords[i].moveTimer += deltaTime;
            if (movedWords[i].moveTimer >= BUG_MOVE_DURATION) {
                movedWords[i].isMoved = false;
                movedWords[i].moveTimer = 0.0f;
            }
        }
    }
}
*/
void UpdateGlitchBug(float deltaTime) {
    for (int i = 0; i < movedLettersCount; i++) {
        if (!movedLetters[i].isMoved && movedLetters[i].moveTimer == 0.0f) {
            float offsetX = (rand() % (BUG_MOVE_RANGE_X * 2 + 1)) - BUG_MOVE_RANGE_X;
            float offsetY = (rand() % (BUG_MOVE_RANGE_Y * 2 + 1)) - BUG_MOVE_RANGE_Y;
            movedLetters[i].movePosition = (Vector2){
                movedLetters[i].initialPosition.x + offsetX,
                movedLetters[i].initialPosition.y + offsetY
            };
            movedLetters[i].isMoved = true;
        }

        if (movedLetters[i].isMoved) {
            movedLetters[i].moveTimer += deltaTime;
            if (movedLetters[i].moveTimer >= BUG_MOVE_DURATION) {
                movedLetters[i].isMoved = false;
                movedLetters[i].moveTimer = 0.0f;
            }
        }
    }
}

/*
void DrawGlitchBug(Font font, float scrollOffset) {
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
            bool bugged = false;

            for (int k = 0; k < movedWordsCount; k++) {
                if (movedWords[k].lineIndex == i && movedWords[k].wordIndexInLine == j) {
                    Vector2 pos = movedWords[k].isMoved ? movedWords[k].movePosition : movedWords[k].initialPosition;
                    pos.y += scrollOffset;
                    DrawTextEx(font, tokens[j], pos, FONT_SIZE, 2, COLOR_BUG);
                    bugged = true;
                    break;
                }
            }

            if (!bugged) {
                DrawTextEx(font, tokens[j], (Vector2){xPos, yPos}, FONT_SIZE, 2, COLOR_TEXT);
            }

            xPos += MeasureTextEx(font, tokens[j], FONT_SIZE, 2).x + WORD_SPACING;
            free(tokens[j]);
        }

        free(tmp);
    }
}
*/

void DrawGlitchBug(Font font, float scrollOffset) {
    for (int i = 0; i < lineCount; i++) {
        char *line = lines[i];
        float xPos = TEXT_MARGIN_LEFT;
        float yPos = TEXT_MARGIN_TOP + i * LINE_HEIGHT + scrollOffset;

        for (int j = 0; line[j] != '\0'; j++) {
            if (line[j] == '\n') continue;

            bool bugged = false;

            for (int k = 0; k < movedLettersCount; k++) {
                if (movedLetters[k].lineIndex == i && movedLetters[k].charIndexInLine == j) {
                    Vector2 pos = movedLetters[k].isMoved ? movedLetters[k].movePosition : movedLetters[k].initialPosition;
                    pos.y += scrollOffset;
                    char ch[2] = { movedLetters[k].character, '\0' };
                    DrawTextEx(font, ch, pos, FONT_SIZE, 2, COLOR_BUG);
                    bugged = true;
                    break;
                }
            }

            if (!bugged) {
                char ch[2] = { line[j], '\0' };
                DrawTextEx(font, ch, (Vector2){xPos, yPos}, FONT_SIZE, 2, COLOR_TEXT);
            }

            Vector2 charSize = MeasureTextEx(font, "A", FONT_SIZE, 2);
            xPos += charSize.x;
        }
    }
}

