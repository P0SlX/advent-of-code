#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

int main() {
    clock_t startTime = clock(), diff;

    // Lecture du fichier
    FILE *fptr = fopen("day3.txt", "r");
    if (fptr == NULL) {
        fprintf(stderr, "Fichier introuvable\n");
        return 1;
    }

    // Alloue la map
    int mapWidth = 0, mapHeight = 0;
    char **map = NULL;

// Lit le fichier ligne par ligne
    char line[1000];
    while (fgets(line, sizeof(line), fptr)) {
        // Suppression du caractère de fin de ligne
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }

        // On récupère la taille de la map
        if (mapWidth == 0) {
            mapWidth = len;
        }
        mapHeight++;

        // On alloue la ligne
        map = realloc(map, mapHeight * sizeof(char *));
        map[mapHeight - 1] = malloc((mapWidth + 1) * sizeof(char));  // +1 pour le caractère nul

        // On copie la ligne dans la map
        strncpy(map[mapHeight - 1], line, mapWidth);
        map[mapHeight - 1][mapWidth] = '\0';
    }


    fclose(fptr);

    int total = 0;

    // On parcourt la map
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            // On ne s'occupe pas des cases vides
            if (map[y][x] == '.') continue;

            // On récupère le nombre
            int number = strtol(&map[y][x], NULL, 10);
            if (number == 0) continue;

            //
            int numberLength = snprintf(NULL, 0, "%d", number);

            bool needToBeAdded = false;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= numberLength; j++) {
                    // En dehors de la map ? on passe au suivant
                    if (y + i < 0 || y + i >= mapHeight || x + j < 0 || x + j >= mapWidth) continue;

                    const char *str = &map[y + i][x + j];
                    if (*str != '.' && !isdigit(*str)) {
                        needToBeAdded = true;
                    }
                }
            }

            if (needToBeAdded) {
                total += abs(number);
            }

            // Avance x juste après le nombre
            x += numberLength - 1;
        }
    }

    // On libère ~~lacrim~~ la map
    for (int i = 0; i < mapHeight; ++i) {
        free(map[i]);
    }
    free(map);

    diff = clock() - startTime;
    printf("Temps d'exécution %ld nanoseconds\n", diff);

    printf("Résultat : %d\n", total);

    return 0;
}
