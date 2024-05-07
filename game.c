#include "game.h"
#include <stdio.h>
#include <stdbool.h>


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
    for (int team = 0; team < 2; team++) {
        for (int i = 0; i < NUM_UNITS; i++) {
            if (units[team][i].health > 0) { // Unité toujours en vie
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