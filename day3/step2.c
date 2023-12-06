#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

int retrieveNumber(char *array, int position) {
    int taille = (int) strlen(array);

    // Recherche à gauche
    int debut = position;
    while (debut > 0 && isdigit(array[debut - 1])) {
        debut--;
    }

    // Recherche à droite
    int fin = position;
    while (fin < taille - 1 && isdigit(array[fin + 1])) {
        fin++;
    }

    // Création d'une sous-chaîne pour le nombre
    int longueur = fin - debut + 1;
    char nombreStr[20]; // Taille fixe suffisamment grande
    strncpy(nombreStr, array + debut, longueur);
    nombreStr[longueur] = '\0';

    // Conversion de la chaîne en entier
    return (int) strtol(nombreStr, NULL, 10);;
}


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
    int numberViewed[6] = {0, 0, 0, 0, 0, 0};
    int currentIndex = 0;

    // On parcourt la map
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            // On ne veut que les cases avec une étoile
            if (map[y][x] != '*') continue;

            // On va regarder les 8 cases autour, si on trouve un nombre, on le reconstruit
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    // Case actuelle
                    if (dx == 0 && dy == 0) continue;

                    // Out of bounds
                    if (x + dx < 0 || x + dx >= mapWidth || y + dy < 0 || y + dy >= mapHeight) continue;

                    // Case vide
                    if (map[y + dy][x + dx] == '.') continue;

                    int number = retrieveNumber(map[y + dy], x + dx);

                    bool alreadyInList = false;
                    for (int i = 0; i < currentIndex; ++i) {
                        if (numberViewed[i] == number) {
                            alreadyInList = true;
                            break;
                        }
                    }
                    if (alreadyInList) continue;

                    numberViewed[currentIndex] = number;
                    currentIndex++;
                }
            }


            int tmp = 1;
            if (currentIndex > 1) {
                for (int i = 0; i < currentIndex; ++i) {
                    tmp *= numberViewed[i];
                    numberViewed[i] = 0;
                }
                total += tmp;
            }
            currentIndex = 0;
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
