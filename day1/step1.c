#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

int main() {
    clock_t start = clock(), diff;
    int total = 0;
    char line[1000];

    // Lecture du fichier
    FILE *fptr;
    fptr = fopen("day1.txt", "r");

    if (fptr == NULL) {
        printf("Fichier introuvable\n");
        return 1;
    }

    while (fgets(line, sizeof(line), fptr)) {
        int firstDigit = -1, lastDigit = -1;
        unsigned long len = strlen(line);

        for (int i = 0; i < len; ++i) {
            if (isdigit(line[i])) {
                if (firstDigit == -1) firstDigit = line[i] - '0';
                lastDigit = line[i] - '0';
            }
        }

        if (firstDigit != -1 && lastDigit != -1) {
            total += firstDigit * 10 + lastDigit;
        }
    }
    diff = clock() - start;
    printf("Temps d'exécution %ld nanoseconds\n", diff);

    printf("Résultat : %d\n", total);
    fclose(fptr);
    return 0;
}