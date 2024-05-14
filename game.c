#include "game.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
 
Unit units[NUM_TEAMS][NUM_UNITS];
 
 
void initializeUnits(Unit units[2][NUM_UNITS]) {
    // Initialisation des unités pour chaque joueur
    // Joueur 1 (En supposant que 'a' = 0, 'b' = 1, ...)
    units[0][0] = (Unit){'A', 'A', 5, 3, 2, 2, 1, false, 1, 1}; // Col 1, Row 1 (b1)
    units[0][1] = (Unit){'S', 'B', 3, 4, 1, 2, 3, false, 2, 1}; // Col 2, Row 1 (b3)
    units[0][2] = (Unit){'F', 'C', 4, 5, 0, 4, 1, false, 1, 3}; // Col 1, Row 3 (d1)
    units[0][3] = (Unit){'S', 'D', 5, 3, 2, 2, 1, false, 3, 3}; // Col 5, Row 3 (d5)

    // Joueur 2
    units[1][0] = (Unit){'S', 'W', 5, 3, 2, 2, 1, false, 3, 1}; // Col 3, Row 1 (b3)
    units[1][1] = (Unit){'A', 'X', 3, 4, 1, 2, 3, false, 5, 1}; // Col 5, Row 1 (b5)
    units[1][2] = (Unit){'S', 'Y', 4, 5, 0, 4, 1, false, 3, 3}; // Col 3, Row 3 (d3)
    units[1][3] = (Unit){'F', 'Z', 5, 3, 2, 2, 1, false, 5, 3}; // Col 5, Row 3 (d5)
}
 
 
void printBoard(Unit units[2][NUM_UNITS]) {
    char board[ROWS][COLS] = {{' '}};
    // Placer les unites sur le plateau
    for (int team = 0; team < 2; team++) {
        for (int i = 0; i < NUM_UNITS; i++) {
            if (units[team][i].health > 0) { // Unite toujours en vie
                board[units[team][i].posY][units[team][i].posX] = units[team][i].type;
            }
        }
    }
    // Afficher la grille
    printf("  ");
    for (int col = 0; col < COLS; col++) {
        printf("%d ", col);
    }
    printf("\n");
    for (int row = 0; row < ROWS; row++) {
        printf("%d ", row);
        for (int col = 0; col < COLS; col++) {
            printf("|%c", board[row][col]);
        }
        printf("|\n");
    }
}
 
 
bool moveUnit(int team, int unitIndex, int targetX, int targetY) {
    Unit *unit = &units[team][unitIndex];
    int dx = abs(unit->posX - targetX);
    int dy = abs(unit->posY - targetY);
 
    // Verifier si la cible est à portee de deplacement
    if (dx + dy <= unit->movement 
    && targetX >= 0 
    && targetX < COLS 
    && targetY >= 0 
    && targetY < ROWS) {
        unit->posX = targetX;
        unit->posY = targetY;
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
 
bool isInRange(Unit *attacker, Unit *defender) {
    int dx = attacker->posX - defender->posX;
    int dy = attacker->posY - defender->posY;
    return (dx * dx + dy * dy) <= (attacker->range * attacker->range);
}
 
bool attackUnit(int team, int unitIndex, int targetX, int targetY) {
    Unit *attacker = &units[team][unitIndex];
    Unit *defender = NULL;
 
    // Trouver l'unité cible
    for (int t = 0; t < 2; t++) {
        for (int i = 0; i < NUM_UNITS; i++) {
            if (units[t][i].posX == targetX && units[t][i].posY == targetY && t != team) {
                defender = &units[t][i];
                break;
            }
        }
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
 