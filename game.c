#include "game.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

Unit units[NUM_TEAMS][NUM_UNITS];


void initializeUnits(Unit units[2][NUM_UNITS]) {
    // Initialisation des unités pour chaque joueurs
    // Joueur 1 
    units[0][0] = (Unit){'A','S', 5, 3, 2, 2, 1, false, 0, 1}; 
    units[0][1] = (Unit){'B','S',3, 4, 1, 2, 3, false, 1, 1}; 
    units[0][2] = (Unit){'C', 'F',4, 5, 0, 2, 1, false, 0, 3}; 
    units[0][3] = (Unit){'D','F', 5, 3, 2, 2, 1, false, 1, 3}; 
    // Joueur 2
    units[1][0] = (Unit){'W','f',5, 3, 2, 2, 1, false, 5, 1}; 
    units[1][1] = (Unit){'X','f',3, 4, 1, 2, 3, false, 6, 1}; 
    units[1][2] = (Unit){'Y', 'F',4, 5, 0, 4, 1, false, 5, 3}; 
    units[1][3] = (Unit){'Z', 'F',5, 3, 2, 2, 1, false, 6, 3}; 
}


void printBoard(Unit units[2][NUM_UNITS]) {
    char board[ROWS][COLS] = {{' '}};
    char colName[5]={'a','b','c','d','e'};
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = ' ';
        }
    }
    // Placer les unites sur le plateau
    for (int team = 0; team < 2; team++) {
        for (int i = 0; i < NUM_UNITS; i++) {
            // On vérifie si l'unité est toujours en vie
            if (units[team][i].health > 0) { 
                board[units[team][i].posY][units[team][i].posX] = units[team][i].name;
            }
        }
    }
    // Afficher la grille
    printf("   ");
    for (int col = 0; col < COLS; col++) {
        printf("%d ", col);
    }
    printf("\n");
    for (int row = 0; row < ROWS; row++) {
        printf("%c ", colName[row]);
        for (int col = 0; col < COLS; col++) {
            printf("|%c", board[row][col]);
        }
        printf("|\n");
    }
}


bool moveUnit(Unit units[NUM_TEAMS][NUM_UNITS], int team, char name, int targetX, int targetY) {
    for (int i = 0; i < NUM_UNITS; i++) {
        Unit *unit = &units[team][i];
        if (unit->name == name) {
            int dx = abs(unit->posX - targetX);
            int dy = abs(unit->posY - targetY);

            // Vérifier si la case cibble est à portée
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

bool attackUnit(Unit units[NUM_TEAMS][NUM_UNITS], int team, char atkName, char targetName) {
    printf("attaque en cours.. \n");
    // Définition de l'équipe adverse
    int adv = team == 0 ? 1 : 0;

    // Trouver l'unité attaquant
    Unit *attacker;
    for(int i = 0; i < NUM_UNITS; i++){
        Unit *unit = &units[team][i];
        if(unit->name = atkName){
            *attacker = *unit;
        }
    }
    printf("%c", attacker->name);

    // Trouver l'unité cible
    Unit *defender;
    for(int i = 0; i < NUM_UNITS; i++){
        Unit *unit = &units[adv][i];
        if(unit->name = targetName){
            *defender = *unit;
        }
    }
    printf("%c", defender->name);

 
 
    if (defender && isInRange(attacker, defender)) {
        int damage = attacker->attack - defender->defense;
        damage = damage > 0 ? damage : 1; // Le dégât minimum est de 1
        defender->health -= damage;
        printf("%d", damage);
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

bool checkEndGame(Unit units[NUM_TEAMS][NUM_UNITS]) {
    bool teamAlive[2] = {false, false};

    for (int team = 0; team < 2; team++) {
        for (int i = 0; i < NUM_UNITS; i++) {
            if (units[team][i].health > 0) {
                teamAlive[team] = true;
                break;
            }
        }
    }

    
    return !(teamAlive[0] && teamAlive[1]);
}

int determineWinner(Unit units[NUM_TEAMS][NUM_UNITS]) {
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