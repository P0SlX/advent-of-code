#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_CARDS 196
#define WINNING_NUMBERS 10
#define MY_NUMBERS 25

typedef struct Node {
    int winning_numbers[WINNING_NUMBERS];
    int my_numbers[MY_NUMBERS];
    struct Node *next;
} Node;

Node* create_node(int *winning_numbers, int *my_numbers) {
    Node *node = (Node *)malloc(sizeof(Node));
    memcpy(node->winning_numbers, winning_numbers, WINNING_NUMBERS * sizeof(int));
    memcpy(node->my_numbers, my_numbers, MY_NUMBERS * sizeof(int));
    node->next = NULL;
    return node;
}

Node* read_scratchcards(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    Node *head = NULL, *tail = NULL;
    char line[1024];
    int winning_numbers[WINNING_NUMBERS], my_numbers[MY_NUMBERS];

    while (fgets(line, sizeof(line), file)) {
        // Que c'est pas beau mais ca marche super bien :)
        sscanf(line, "Card %d: %d %d %d %d %d %d %d %d %d %d | %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
               &winning_numbers[0],
               &winning_numbers[0], &winning_numbers[1], &winning_numbers[2], &winning_numbers[3], &winning_numbers[4],
               &winning_numbers[5], &winning_numbers[6], &winning_numbers[7], &winning_numbers[8], &winning_numbers[9],
               &my_numbers[0], &my_numbers[1], &my_numbers[2], &my_numbers[3], &my_numbers[4],
               &my_numbers[5], &my_numbers[6], &my_numbers[7], &my_numbers[8], &my_numbers[9],
               &my_numbers[10], &my_numbers[11], &my_numbers[12], &my_numbers[13], &my_numbers[14],
               &my_numbers[15], &my_numbers[16], &my_numbers[17], &my_numbers[18], &my_numbers[19],
               &my_numbers[20], &my_numbers[21], &my_numbers[22], &my_numbers[23], &my_numbers[24]);

        Node *node = create_node(winning_numbers, my_numbers);
        if (head == NULL) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }

    fclose(file);
    return head;
}

bool contains(const int *array, int size, int number) {
    for (int i = 0; i < size; i++) {
        if (array[i] == number) {
            return true;
        }
    }
    return false;
}

int count_matches(Node *node) {
    int matches = 0;
    for (int i = 0; i < MY_NUMBERS; i++) {
        if (contains(node->winning_numbers, WINNING_NUMBERS, node->my_numbers[i])) {
            matches++;
        }
    }
    return matches;
}

// Function to process scratchcards
int process_scratchcards(Node *head) {
    int card_counts[TOTAL_CARDS] = {0};

    // Initialise tous les éléments à 1
    for (int i = 0; i < TOTAL_CARDS; i++) {
        card_counts[i] = 1;
    }

    Node *node = head;
    int i = 0;

    while (node != NULL) {
        int matches = count_matches(node);

        for (int j = i + 1; j < i + 1 + matches && j < TOTAL_CARDS; j++) {
            card_counts[j] += card_counts[i];
        }

        node = node->next;
        i++;
    }

    int total = 0;
    for (int j = 0; j < TOTAL_CARDS; j++) {
        total += card_counts[j];
    }

    return total;
}

int main() {
    clock_t startTime = clock();

    const char *file_path = "day4.txt";
    Node *head = read_scratchcards(file_path);
    int total_scratchcards = process_scratchcards(head);


    clock_t diff = clock() - startTime;
    printf("Résultat: %d\n", total_scratchcards);
    printf("Execution time: %ld nanoseconds\n", diff);

    // Libération les noeuds
    Node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}
