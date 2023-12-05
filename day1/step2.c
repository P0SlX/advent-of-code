#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <regex.h>

// Reverse une string
void strrev(char* str) {
    if (!str) {
        return;
    }

    int i = 0;
    unsigned int j = strlen(str) - 1;

    while (i < j) {
        char c = str[i];
        str[i] = str[j];
        str[j] = c;
        i++;
        j--;
    }
}

int wordToDigit(const char *word) {
    switch (strlen(word)) {
        case 3: // "one", "two", "six"
            if (strcmp(word, "one") == 0) return 1;
            if (strcmp(word, "two") == 0) return 2;
            if (strcmp(word, "six") == 0) return 6;
            break;
        case 4: // "four", "five", "nine"
            if (strcmp(word, "four") == 0) return 4;
            if (strcmp(word, "five") == 0) return 5;
            if (strcmp(word, "nine") == 0) return 9;
            break;
        case 5: // "three", "seven", "eight"
            if (strcmp(word, "three") == 0) return 3;
            if (strcmp(word, "seven") == 0) return 7;
            if (strcmp(word, "eight") == 0) return 8;
            break;
        default:
            return -1;
    }
    return -1;
}

long extractDigits(char *line, regex_t regex, bool reversed) {
    int ret;
    regmatch_t match;

    if (reversed) {
        strrev(line);
    }

    ret = regexec(&regex, line, 1, &match, 0);
    if (!ret) {
        char matched_str[match.rm_eo - match.rm_so + 1];
        strncpy(matched_str, line + match.rm_so, match.rm_eo - match.rm_so);
        matched_str[match.rm_eo - match.rm_so] = '\0';

        if (isdigit(matched_str[0])) {
            return strtol(matched_str, NULL, 10);
        } else {
            // Reverse le nombre si on a reverse la string initialement
            if (reversed) {
                strrev(matched_str);
            }
            int digit = wordToDigit(matched_str);
            if (digit != -1) {
                return digit;
            }
        }
    } else if (ret != REG_NOMATCH) {
        fprintf(stderr, "Pas de match\n");
    }

    exit(1);
}

int main() {
    clock_t start = clock(), diff;
    char line[1000];
    long total = 0;

    // Lecture du fichier
    FILE *fptr;
    fptr = fopen("day1.txt", "r");

    if (fptr == NULL) {
        printf("Fichier introuvable\n");
        return 1;
    }

    // Compilation des deux regex
    regex_t regex;
    regex_t regexReverse;
    char *pattern = "(one|two|three|four|five|six|seven|eight|nine|[0-9])";
    char *patternReverse = "(eno|owt|eerht|ruof|evif|xis|neves|thgie|enin|[0-9])";

    int ret = regcomp(&regex, pattern, REG_EXTENDED);
    int ret2 = regcomp(&regexReverse, patternReverse, REG_EXTENDED);

    if (ret || ret2) {
        fprintf(stderr, "Impossible de compiler la/les regex\n");
        exit(1);
    }

    while (fgets(line, sizeof(line), fptr)) {
        long firstDigit = extractDigits(line, regex, false);
        long lastDigit = extractDigits(line, regexReverse, true);
        total += firstDigit * 10 + lastDigit;
    }
    regfree(&regex);
    regfree(&regexReverse);


    diff = clock() - start;
    printf("Temps d'exécution %ld nanoseconds\n", diff);

    printf("Résultat : %d\n", total);
    return 0;
}