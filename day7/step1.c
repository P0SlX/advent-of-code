#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HANDS 1000
#define LINE_BUFFER_SIZE 100

typedef struct {
    char cards[5];
    int bid;
    int rank;
    int handType;
} Hand;

int cardValue(char card) {
    if (card >= '2' && card <= '9') return card - '2';
    switch (card) {
        case 'T': return 8;
        case 'J': return 9;
        case 'Q': return 10;
        case 'K': return 11;
        case 'A': return 12;
        default:
            fprintf(stderr, "Carte invalide détectée : %c\n", card);
            return -1;
    }
}

void countCards(const char *hand, int *counts) {
    for (int i = 0; i < 13; i++) {
        counts[i] = 0;
    }

    for (int i = 0; i < 5; i++) {
        if (hand[i] >= '2' && hand[i] <= '9') {
            counts[hand[i] - '2']++;
        } else {
            switch (hand[i]) {
                case 'T':
                    counts[8]++;
                    break;
                case 'J':
                    counts[9]++;
                    break;
                case 'Q':
                    counts[10]++;
                    break;
                case 'K':
                    counts[11]++;
                    break;
                case 'A':
                    counts[12]++;
                    break;
                default:
                    fprintf(stderr, "Carte invalide détectée : %c\n", hand[i]);
                    break;
            }
        }
    }
}

int determineHandType(const char *hand) {
    int counts[13];
    countCards(hand, counts);

    int pairs = 0, threes = 0, fours = 0, five = 0;
    for (int i = 0; i < 13; i++) {
        if (counts[i] == 5) five++;
        if (counts[i] == 4) fours++;
        if (counts[i] == 3) threes++;
        if (counts[i] == 2) pairs++;
    }

    if (five) return 7;  // Quinte flush
    if (fours) return 6;  // Carré
    if (threes && pairs) return 5;  // Full
    if (threes) return 4;  // Brelan
    if (pairs == 2) return 3;  // Deux paires
    if (pairs) return 2;  // Une paire
    return 1;  // Hauteur
}

int compareHands(const void *a, const void *b) {
    Hand *handA = (Hand *) a;
    Hand *handB = (Hand *) b;

    // Types de mains différents ?
    if (handA->handType != handB->handType) {
        return handB->handType - handA->handType;
    }

    for (int i = 0; i < 5; i++) {
        int valueA = cardValue(handA->cards[i]);
        int valueB = cardValue(handB->cards[i]);
        if (valueA != valueB) {
            return valueB - valueA;
        }
    }

    return 0;
}

int readInput(Hand *hands) {
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
    return count;
}

int calculateWinnings(Hand *hands, int size) {
    int total = 0;
    for (int i = 0; i < size; i++) {
        hands[i].rank = size - i;
        total += hands[i].bid * hands[i].rank;
    }
    return total;
}

int main() {
    Hand *hands = malloc(MAX_HANDS * sizeof(Hand));

    int count = readInput(hands);
    for (int i = 0; i < count; i++) {
        hands[i].handType = determineHandType(hands[i].cards);
    }

    qsort(hands, count, sizeof(Hand), compareHands);

    int totalWinnings = calculateWinnings(hands, count);
    printf("Total winnings: %d\n", totalWinnings);
    free(hands);
    return 0;
}