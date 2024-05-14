#include <stdio.h>
#include <stdbool.h>
#include "game.h"
 
#include <stdio.h>
#include <stdlib.h>
 
 
int main() {
    Unit units[NUM_TEAMS][NUM_UNITS]; // Deux équipes de 4 unités chacune
    initializeUnits(units);   // Initialiser les unités sur le plateau
    printBoard(units);

    // Déplacer une unité
    if (moveUnit(units, 0, 'B', 3, 1)) {
        printf("Deplacement reussi\n");
    } else {
        printf("Deplacement echoue\n");
    }
    printBoard(units);

    if(attackUnit(units, 0, 'B', 'W')){
        printf("attaque reussie");
    }
    else{
        printf("attaque ratee");
    }

    return 0;
}