#include "bug_move.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

// === PARAMÈTRES DU BUG ===
#define BUG_PERCENTAGE         20       // % de mots à buguer
#define BUG_MOVE_DURATION      0.0001f     // Durée du bug (secondes)
#define BUG_MOVE_RANGE_X       2        // Décalage max horizontal en pixels
#define BUG_MOVE_RANGE_Y       2
Color COLOR_BUG = (Color){211, 211, 211, 255};
// === PARAMÈTRES DE MISE EN PAGE ===
#define LINE_HEIGHT            40       // Hauteur entre les lignes
#define FONT_SIZE              30       // Taille du texte
#define WORD_SPACING           10       // Espace entre les mots
#define TEXT_MARGIN_LEFT       500      // Marge gauche
#define TEXT_MARGIN_TOP        0
#define SCREEN_WIDTH           400
Color COLOR_TEXT = (Color){255, 255, 255, 255};



char *lines[MAX_LINES];
int lineCount = 0;

typedef struct {
    int lineIndex;
    int wordIndexInLine;
    char text[64];
    Vector2 initialPosition;
    Vector2 movePosition;
    bool isMoved;
    float moveTimer;
} WordMoveInfo;

WordMoveInfo movedWords[MAX_MOVED_WORDS];
int movedWordsCount = 0;


void LoadTextFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur d'ouverture du fichier texte");
        exit(1);
    }

    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, MAX_LINE_LENGTH, file)) {
        if (lineCount >= MAX_LINES) break;

        // Affichage du contenu pour déboguer
        printf("Ligne lue: %s", buffer);

        lines[lineCount] = strdup(buffer);
        if (!lines[lineCount]) {
            printf("Erreur d'allocation mémoire\n");
            exit(1);
        }
        lineCount++;
    }

    fclose(file);
}


void ResetMovedWords() {
    for (int i = 0; i < MAX_MOVED_WORDS; i++) {
        movedWords[i].isMoved = false;
        movedWords[i].moveTimer = 0.0f;
        movedWords[i].initialPosition = (Vector2){0, 0};
        movedWords[i].movePosition = (Vector2){0, 0};
    }
}

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

void UpdateMovedWords(float deltaTime) {
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

void DrawTextWithBug(Font font, float deltaTime, float scrollOffset) {
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

