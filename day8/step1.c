#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_LENGTH 732
#define MAX_LINE_LENGTH 280

typedef struct {
    char string[4];
    char left[4];
    char right[4];
} Node;

typedef struct {
    Node *nodes;
    int *starts;
    int sizes[2];
} Graph;

void get_nodes(char *arr[], Graph *graph);

int search_string(Graph *graph, char *str);

int find_steps(Graph *graph, char *instructions, int pos_start);

void read_file(char *arr[]) {
    FILE *fptr = fopen("day8.txt", "r");
    if (!fptr) {
        fprintf(stderr, "Fichier introuvable\n");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    int index = 0;
    while (fgets(line, sizeof(line), fptr)) {
        int end_line = 0;
        for (; line[end_line] != '\n'; end_line++) {}
        strncpy(arr[index], line, end_line);
        index++;
    }
    fclose(fptr);
}

int main() {
    clock_t start_time = clock();
    char *values[FILE_LENGTH];

    // Allocation de la mémoire pour les lignes du fichier
    for (int i = 0; i < FILE_LENGTH; i++) {
        values[i] = calloc(MAX_LINE_LENGTH, sizeof(char));
        if (!values[i]) {
            fprintf(stderr, "Impossible d'allouer de la mémoire pour values\n");
            exit(EXIT_FAILURE);
        }
    }

    read_file(values);

    // Récupération des instructions de la première ligne
    char instructions[strlen(values[0]) + 1];
    strcpy(instructions, values[0]);

    // Création et initialisation du graphe
    Graph graph = {NULL, NULL, {0, 0}};
    get_nodes(values, &graph);

    // Calcul des étapes
    int steps = find_steps(&graph, instructions, graph.starts[0]);

    // Calcul et affichage du temps d'exécution
    clock_t diff = clock() - start_time;
    printf("Temps d'exécution: %ld microsecondes\n", diff / 1000);
    printf("Étapes: %d\n", steps);

    // Clean tout ca
    for (int i = 0; i < FILE_LENGTH; i++) {
        free(values[i]);
    }
    free(graph.nodes);
    free(graph.starts);

    return 0;
}

void get_nodes(char *arr[], Graph *graph) {
    for (int i = 2; i < FILE_LENGTH; i++) {
        Node *temp_nodes = realloc(graph->nodes, (graph->sizes[0] + 1) * sizeof(Node));
        if (!temp_nodes) {
            fprintf(stderr, "Impossible d'allouer temp_nodes\n");
            exit(EXIT_FAILURE);
        }
        graph->nodes = temp_nodes;

        strncpy(graph->nodes[graph->sizes[0]].string, arr[i], 3);
        strncpy(graph->nodes[graph->sizes[0]].left, &arr[i][7], 3);
        strncpy(graph->nodes[graph->sizes[0]].right, &arr[i][12], 3);
        graph->nodes[graph->sizes[0]].string[3] = '\0';
        graph->nodes[graph->sizes[0]].left[3] = '\0';
        graph->nodes[graph->sizes[0]].right[3] = '\0';
        graph->sizes[0]++;
    }
    graph->starts = malloc(sizeof(int));
    if (!graph->starts){
        fprintf(stderr, "Impossible de réallouer le point de départ du graph\n");
        exit(EXIT_FAILURE);
    }
    graph->starts[0] = search_string(graph, "AAA");
    graph->sizes[1]++;
}

int search_string(Graph *graph, char *str) {
    for (int i = 0; i < graph->sizes[0]; i++) {
        if (strcmp(graph->nodes[i].string, str) == 0) return i;
    }
    return -1;
}

int find_steps(Graph *graph, char *instructions, int pos_start) {
    int steps = 0, pos = pos_start, index_inst = 0;
    size_t len = strlen(instructions);

    while (graph->nodes[pos].string[2] != 'Z') {
        pos = search_string(graph, (instructions[index_inst++] == 'L') ? graph->nodes[pos].left : graph->nodes[pos].right);
        if (index_inst == len) index_inst = 0;
        steps++;
    }
    return steps;
}
