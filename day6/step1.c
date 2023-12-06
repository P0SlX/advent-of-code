#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

int calculateWays(int raceTime, int recordDistance) {
    int ways = 0;
    for (int holdTime = 1; holdTime < raceTime; holdTime++) {
        int speed = holdTime;
        int travelTime = raceTime - holdTime;
        int distance = speed * travelTime;
        if (distance > recordDistance) {
            ways++;
        }
    }
    return ways;
}

int main() {
    clock_t startTime = clock(), diff;

    int times[4] = {47, 70, 75, 66};
    int distances[4] = {282, 1079, 1147, 1062};

    int numberOfRaces = sizeof(times) / sizeof(times[0]);
    int totalWays = 1;

    for (int i = 0; i < numberOfRaces; i++) {
        int ways = calculateWays(times[i], distances[i]);
        totalWays *= ways;
    }


    diff = clock() - startTime;
    printf("Temps d'exécution %ld nanoseconds\n", diff);
    printf("Résultat : %d\n", totalWays);
    return 0;
}
