#include "game.h"
#include <stdio.h>
#include <stdbool.h>
<<<<<<< Updated upstream


void initializeUnits(Unit units[2][NUM_UNITS]) {
    // Initialisation des unités pour chaque joueur
    // Joueur 1
    units[0][0] = (Unit){'S', 5, 3, 2, 2, 1, false, 0, 1};
    units[0][1] = (Unit){'A', 3, 4, 1, 2, 3, false, 6, 1};
    units[0][2] = (Unit){'F', 4, 5, 0, 4, 1, false, 0, 3};
    units[0][3] = (Unit){'S', 5, 3, 2, 2, 1, false, 6, 3};
    // Joueur 2
    units[1][0] = (Unit){'S', 5, 3, 2, 2, 1, false, 0, 1};
    units[1][1] = (Unit){'A', 3, 4, 1, 2, 3, false, 6, 1};
    units[1][2] = (Unit){'F', 4, 5, 0, 4, 1, false, 0, 3};
    units[1][3] = (Unit){'S', 5, 3, 2, 2, 1, false, 6, 3};
}

void printBoard(Unit units[2][NUM_UNITS]) {
    char board[ROWS][COLS] = {{' '}};
    // Placer les unités sur le plateau
=======
#include <stdlib.h>
#include <string.h>
 
Unit units[NUM_TEAMS][NUM_UNITS];
 
 
void initializeUnits(Unit units[2][NUM_UNITS]) {
    // Initialisation des unités pour chaque joueur
    // Joueur 1 (En supposant que 'a' = 0, 'b' = 1, ...)
    units[0][0] = (Unit){'A', 'A', 5, 3, 2, 2, 1, false, 0, 1}; // Col 1, Row 1 (b1)
    units[0][1] = (Unit){'S', 'B', 3, 4, 1, 2, 3, false, 1, 1}; // Col 2, Row 1 (b3)
    units[0][2] = (Unit){'F', 'C', 4, 5, 0, 4, 1, false, 0, 3}; // Col 1, Row 3 (d1)
    units[0][3] = (Unit){'S', 'D', 5, 3, 2, 2, 1, false, 1, 3}; // Col 5, Row 3 (d5)

    // Joueur 2
    units[1][0] = (Unit){'S', 'W', 5, 3, 2, 2, 1, false, 5, 1}; // Col 3, Row 1 (b3)
    units[1][1] = (Unit){'A', 'X', 3, 4, 1, 2, 3, false, 6, 1}; // Col 5, Row 1 (b5)
    units[1][2] = (Unit){'S', 'Y', 4, 5, 0, 4, 1, false, 5, 3}; // Col 3, Row 3 (d3)
    units[1][3] = (Unit){'F', 'Z', 5, 3, 2, 2, 1, false, 6, 3}; // Col 5, Row 3 (d5)
}


void printBoard(Unit units[2][NUM_UNITS]) {
    char board[ROWS][COLS] = {{' '}};
    char rowName[5] = {'a', 'b', 'c', 'd', 'e'};
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = ' ';
        }
    } 
    // Placer les unites sur la grille
>>>>>>> Stashed changes
    for (int team = 0; team < 2; team++) {
        for (int i = 0; i < NUM_UNITS; i++) {
            if (units[team][i].health > 0) { // Unité toujours en vie
                board[units[team][i].posY][units[team][i].posX] = units[team][i].type;
            }
        }
    }
    // Afficher la grille
    printf("   ");
    for (int col = 1; col <= COLS; col++) {
        printf("%d ", col);
    }
    printf("\n");
    for (int row = 0; row < ROWS; row++) {
        printf("%c ", rowName[row]);
        for (int col = 0; col < COLS; col++) {
            printf("|%c", board[row][col]);
        }
        printf("|\n");
    }
}
<<<<<<< Updated upstream

void moveUnit(Unit *unit, int targetX, int targetY) {
    // Supposition : la vérification de la validité du déplacement est déjà faite
    unit->posX = targetX;
    unit->posY = targetY;
    unit->isFatigued = true;  // L'unité devient fatiguée après le mouvement
}

bool isInRange(Unit *attacker, Unit *defender) {
    int dx = attacker->posX - defender->posX;
    int dy = attacker->posY - defender->posY;
    return (dx * dx + dy * dy) <= (attacker->range * attacker->range);
}

void attackUnit(Unit *attacker, Unit *defender) {
    if (isInRange(attacker, defender)) {
        int damage = attacker->attack - defender->defense;
        if (damage < 1) damage = 1;  // Le dégât minimal est 1
        defender->health -= damage;
        printf("%c attaque %c pour %d dégâts!\n", attacker->type, defender->type, damage);
        if (defender->health > 0) {
            // Contre-attaque
            damage = defender->attack - attacker->defense;
            if (damage < 1) damage = 1;
            attacker->health -= damage;
            printf("%c contre-attaque %c pour %d dégâts!\n", defender->type, attacker->type, damage);
        }
    }
}

void gameLoop(Unit units[2][NUM_UNITS]) {
    int currentPlayer = 0;
    while (true) {  // Boucle principale du jeu
        printf("Tour du joueur %d\n", currentPlayer + 1);
        printBoard(units);
        // Ajoutez ici la logique de sélection et de mouvement des unités
        // Également la gestion de la fin du jeu (toutes les unités d'une équipe sont éliminées)
        currentPlayer = 1 - currentPlayer;  // Change de joueur
    }
}
=======
 
 
bool moveUnit(Unit units[NUM_TEAMS][NUM_UNITS], int team, char name, int targetX, int targetY) {
    for(int i = 0; i < NUM_UNITS; i++){
        Unit *unit = &units[team][i];
        if(unit->name = name){
            int dx = abs(unit->posX - targetX);
            int dy = abs(unit->posY - targetY);
            
            if (dx + dy <= unit->movement 
            && targetX >= 0 
            && targetX < COLS 
            && targetY >= 0 
            && targetY < ROWS) {
                unit->posX = targetX;
                unit->posY = targetY;
                return true;
            }
        }
    }

    return false;
}

bool isInRange(Unit *attacker, Unit *defender) {
    int dx = attacker->posX - defender->posX;
    int dy = attacker->posY - defender->posY;
    return (dx * dx + dy * dy) <= (attacker->range * attacker->range);
}
 
bool attackUnit(int team, char atkName, char targetName) {
    // Définition de l'équipe adverse
    int adv;
    team == 0 ? adv = 1 : adv = 0;

    // Trouver l'unité attaquant
    Unit *attacker;
    for(int i = 0; i < NUM_UNITS; i++){
        Unit *unit = &units[team][i];
        if(unit->name = atkName){
            *attacker = *unit;
        }

    // Trouver l'unité cible
    Unit *defender;
    for(int i = 0; i < NUM_UNITS; i++){
        Unit *unit = &units[adv][i];
        if(unit->name = targetName){
            *defender = *unit;
        }
 
 
    if (defender && abs(attacker->posX - defender->posX) + abs(attacker->posY - defender->posY) <= attacker->range) {
        int damage = attacker->attack - defender->defense;
        damage = damage > 0 ? damage : 1; // Le dégât minimum est de 1
        defender->health -= damage;
        if (defender->health > 0) {
            // Contre-attaque
            damage = defender->attack - attacker->defense;
            damage = damage > 0 ? damage : 1;
            attacker->health -= damage;
        }
        return true;
    }
 
    return false;
}
 
bool checkEndGame(Unit units[2][NUM_UNITS]) {
    bool teamAlive[2] = {false, false};
 
    for (int team = 0; team < 2; team++) {
        for (int i = 0; i < NUM_UNITS; i++) {
            if (units[team][i].health > 0) {
                teamAlive[team] = true;
                break;
            }
        }
    }
 
    // Si l'une des equipes n'a plus d'unites vivantes, le jeu se termine
    return !(teamAlive[0] && teamAlive[1]);
}
 
int determineWinner(Unit units[2][NUM_UNITS]) {
    int healthSum[2] = {0, 0};
 
    for (int team = 0; team < 2; team++) {
        for (int i = 0; i < NUM_UNITS; i++) {
            healthSum[team] += units[team][i].health;
        }
    }
 
    if (healthSum[0] > healthSum[1]) {
        return 1; // Joueur 1 gagne
    } else if (healthSum[1] > healthSum[0]) {
        return 2; // Joueur 2 gagne
    }
 
    return 0; // egalite, à gerer si necessaire
}
 
>>>>>>> Stashed changes
