#include <stdio.h>
#include <time.h>

#define RACE_TIME 47707566
#define RECORD_DISTANCE 282107911471062

int main() {
    clock_t startTime = clock(), diff;

    long ways = 0;

    for (long holdTime = 1; holdTime < RACE_TIME; holdTime++) {
        long speed = holdTime;
        long travelTime = RACE_TIME - holdTime;
        long distance = speed * travelTime;

        if (distance > RECORD_DISTANCE) {
            ways++;
        }
    }


    diff = clock() - startTime;
    printf("Temps d'exécution %ld microsecondes\n", diff/1000);
    printf("Résultat : %d\n", ways);
    return 0;
}
