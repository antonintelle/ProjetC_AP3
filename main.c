#include <stdio.h>
#include <stdbool.h>
<<<<<<< Updated upstream

int main(){
    
=======
#include "game.h"
 
#include <stdio.h>
#include <stdlib.h>
 
 
int main() {
    Unit units[NUM_TEAMS][NUM_UNITS]; // Deux équipes de 4 unités chacune
    int currentTeam = 0;      // Commencer par l'équipe 0
    int roundCounter = 0;     // Compteur de tours pour la limite de 100 tours
 
    initializeUnits(units);   // Initialiser les unités sur le plateau
    printBoard(units);

    moveUnit(units, currentTeam, 'B', 3, 1);
    printBoard(units);
 
    return 0;
>>>>>>> Stashed changes
}