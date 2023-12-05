#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024

typedef struct {
    int red, green, blue;
} CubeLimits;

void compileRegex(regex_t *regex, const char *pattern) {
    int ret = regcomp(regex, pattern, REG_EXTENDED);
    if (ret) {
        fprintf(stderr, "Impossible de compiler la regex\n");
        exit(1);
    }
}

int extractNumber(const char *str, regex_t *regex) {
    regmatch_t matches[2];
    if (regexec(regex, str, 2, matches, 0) == 0) {
        // T'abuses si tu mets plus de 10 chiffres quand même
        char numStr[10];

        // Début et fin du nombre
        int start = matches[1].rm_so;
        int end = matches[1].rm_eo;

        // Copie le nombre dans numStr
        strncpy(numStr, str + start, end - start);

        // On t'oublie pas toi
        numStr[end - start] = '\0';
        return (int) strtol(numStr, NULL, 10);
    }
    return 0;
}

int main() {
    clock_t start = clock(), diff;
    CubeLimits maxCubes = {12, 13, 14}; // Max de chaque couleur
    int totalSum = 0;

    // Compile les regex
    regex_t redRegex, greenRegex, blueRegex;
    compileRegex(&redRegex, "([0-9]+) red");
    compileRegex(&greenRegex, "([0-9]+) green");
    compileRegex(&blueRegex, "([0-9]+) blue");

    char line[MAX_LINE_LENGTH];

    // Lecture du fichier
    FILE *fptr = fopen("day2.txt", "r");
    if (fptr == NULL) {
        fprintf(stderr, "Fichier introuvable\n");
        return 1;
    }

    while (fgets(line, sizeof(line), fptr)) {
        int gameId, isPossible = 1;
        sscanf(line, "Game %d", &gameId);

        char *token = strtok(line, ";");
        while (token != NULL && isPossible) {
            int red = extractNumber(token, &redRegex);
            int green = extractNumber(token, &greenRegex);
            int blue = extractNumber(token, &blueRegex);

            if (red > maxCubes.red || green > maxCubes.green || blue > maxCubes.blue) {
                isPossible = 0;
            }

            token = strtok(NULL, ";");
        }

        if (isPossible) {
            totalSum += gameId;
        }
    }

    fclose(fptr);

    regfree(&redRegex);
    regfree(&greenRegex);
    regfree(&blueRegex);

    diff = clock() - start;
    printf("Temps d'exécution %ld nanoseconds\n", diff);

    printf("Somme des IDs des jeux possibles: %d\n", totalSum);
    return 0;
}
