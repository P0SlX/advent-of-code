#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LINE_BUFFER_SIZE 16
#define MAX_HANDS 1000

typedef struct {
    char cards[5];
    int bid;
    int rank;
    int handType;
} Hand;

int cardToIndex(const char *card) {
    const char *cardOrder = "J23456789TQKA";
    char *ptr = strchr(cardOrder, *card);
    return ptr ? (int) (ptr - cardOrder) : -1;
}

int cardsStrength(char *cards) {
    char count[13] = {0};
    int i = 5, j = 0, threes = 0, twos = 0;

    // Compte les cartes
    while (i--) {
        if (cards[i] == 'J') j++;
        count[cardToIndex(&cards[i])]++;
    }

    // Détermine la force de la main
    for (i = 0; i < 13; i++) {
        switch (count[i]) {
            case 5:
                return 6;
            case 4:
                return j ? 6 : 5;
            case 3:
                threes++;
                break;
            case 2:
                twos++;
                break;
        }
    }

    if (threes && twos) {
        return j ? 6 : 4;
    }

    if (threes) {
        return j ? 5 : 3;
    }

    if (twos == 2) {
        switch (j) {
            case 1:
                return 4;
            case 2:
                return 5;
            default:
                return 2;
        }
    }

    if (twos == 1) {
        switch (j) {
            case 1:
            case 2:
                return 3;
            default:
                return 1;
        }
    }

    return j ? 1 : 0;
}

int compareHands(const void *in_a, const void *in_b) {
    Hand *a = (Hand *) in_a;
    Hand *b = (Hand *) in_b;

    int a_strength = cardsStrength(a->cards);
    int b_strength = cardsStrength(b->cards);

    if (a_strength != b_strength) return a_strength - b_strength;

    for (int i = 0; i < 5; i++) {
        if (cardToIndex(&a->cards[i]) == cardToIndex(&b->cards[i])) {
            continue;
        }
        return cardToIndex(&a->cards[i]) - cardToIndex(&b->cards[i]);
    }
    return 0;
}

int main(void) {
    clock_t startTime = clock(), diff;
    Hand hands[MAX_HANDS];
    long unsigned win = 0;
    char *line = malloc(LINE_BUFFER_SIZE * sizeof(char));

    FILE *fptr = fopen("day7.txt", "r");
    if (fptr == NULL) {
        fprintf(stderr, "Fichier introuvable\n");
        free(line);
        return 1;
    }

    int count = 0;
    while (fgets(line, LINE_BUFFER_SIZE, fptr)) {
        if (sscanf(line, "%s %d", hands[count].cards, &hands[count].bid) != 2) {
            fprintf(stderr, "Erreur de format de ligne\n");
            continue;
        }
        count++;
        if (count >= MAX_HANDS) break;
    }

    fclose(fptr);
    free(line);

    qsort(hands, count, sizeof(Hand), compareHands);

    // Calcule les gains
    for (int i = 0; i < count; i++) {
        win += (i + 1) * hands[i].bid;
    }

    printf("%lu\n", win);

    diff = clock() - startTime;
    printf("Temps d'exécution %ld nanoseconds\n", diff);
    return 0;
}