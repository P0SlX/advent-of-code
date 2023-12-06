#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <time.h>


#define MAX_NUMBERS 40
#define MAX_LINE_LENGTH 500

void extractNumbers(char *string, int *numbers, int *count) {
    char *token = strtok(string, " ");
    while (token != NULL) {
        numbers[*count] = (int) strtol(token, NULL, 10);
        (*count)++;
        token = strtok(NULL, " ");
    }
}

int main() {
    clock_t startTime = clock(), diff;

    // Lecture du fichier
    FILE *fptr = fopen("day4.txt", "r");
    if (fptr == NULL) {
        fprintf(stderr, "Fichier introuvable\n");
        return 1;
    }

    regex_t regex1, regex2;
    regmatch_t match[2];
    char line[MAX_LINE_LENGTH];
    char numString1[MAX_LINE_LENGTH], numString2[MAX_LINE_LENGTH];
    int total = 0;

    regcomp(&regex1, ":(.*?)[|]", REG_EXTENDED);
    regcomp(&regex2, "[|](.*)", REG_EXTENDED);

    while (fgets(line, sizeof(line), fptr)) {
        int numbers1[MAX_NUMBERS] = {0}, numbers2[MAX_NUMBERS] = {0};
        int count1 = 0, count2 = 0;

        // Regex entre les deux points et le pipe
        if (regexec(&regex1, line, 2, match, 0) == 0) {
            strncpy(numString1, line + match[1].rm_so, match[1].rm_eo - match[1].rm_so);
            numString1[match[1].rm_eo - match[1].rm_so] = '\0';
            extractNumbers(numString1, numbers1, &count1);
        }

        // Regex entre le pipe et la fin de la ligne
        if (regexec(&regex2, line, 2, match, 0) == 0) {
            strncpy(numString2, line + match[1].rm_so, match[1].rm_eo - match[1].rm_so);
            numString2[match[1].rm_eo - match[1].rm_so] = '\0';
            extractNumbers(numString2, numbers2, &count2);
        }

        int cardPoints = 0;

        // Compare les nombres des deux tableaux
        for (int i = 0; i < count1; i++) {
            for (int j = 0; j < count2; j++) {
                if (numbers1[i] == numbers2[j]) {
                    if (cardPoints == 0) {
                        cardPoints = 1;
                    } else {
                        cardPoints *= 2;
                    }
                    break;
                }
            }
        }

        total += cardPoints;
    }

    fclose(fptr);
    regfree(&regex1);
    regfree(&regex2);

    diff = clock() - startTime;
    printf("Temps d'exécution %ld nanoseconds\n", diff);

    printf("Résultat : %d\n", total);
    return 0;
}