#include "gameLoop.h"
#include "gameMotor.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_DEPTH 3 // profondeur de 3 coups pour l'algorithme MinMax

#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

// Fonction principale pour le tour de l'IA.
void aiTurn(int aiTeam) {
    int bestScore = INT_MIN; 
    Unit *bestUnit = NULL; 
    int bestX = -1, bestY = -1; 

    // Parcourt toutes les unités de l'équipe de l'IA
    for (int i = 0; i < NUM_UNITS; i++) {
        // Vérifie si l'unité n'est pas fatiguée
        if (units[aiTeam][i].health > 0 && !units[aiTeam][i].tired) {
            Unit *unit = &units[aiTeam][i];
            evaluateAllMoves(unit, aiTeam, 1 - aiTeam, true, 0, &bestScore, &bestUnit, &bestX, &bestY);
        }
    }

    // Déplace la meilleure unité vers la meilleure position trouvée
    if (bestUnit != NULL) {
        moveUnit(aiTeam, bestUnit->name, bestX, bestY);
        printf("AI moved unit %c to (%d, %d) with score %d\n", bestUnit->name, bestX, bestY, bestScore);
    } else {
        printf("AI could not find a valid move.\n");
    }
}

// Fonction pour parcourir tous les mouvements possibles d'une unité et évaluer leurs scores.
void evaluateAllMoves(Unit *unit, int aiTeam, int currentPlayer, bool isMaximizingPlayer, int depth, int *bestScore, Unit **bestUnit, int *bestX, int *bestY) {
    // Parcourt tous les déplacements possibles de l'unité
    for (int dx = -unit->movement; dx <= unit->movement; dx++) {
        for (int dy = -unit->movement; dy <= unit->movement; dy++) {

            // Vérifie si le déplacement est dans la portée de mouvement de l'unité
            if (abs(dx) + abs(dy) <= unit->movement) {
                int targetX = unit->posX + dx;
                int targetY = unit->posY + dy;
                // Vérifie si le déplacement est valide dans les limites de la grille
                if (targetX >= 0 && targetX < COLS && targetY >= 0 && targetY < ROWS) {
                    int score = evaluateMove(depth, !isMaximizingPlayer, aiTeam, currentPlayer, unit, targetX, targetY);
                    // Met à jour le meilleur ou le pire score en fonction du joueur
                    if (isMaximizingPlayer) {
                        if (score > *bestScore) {
                            *bestScore = score;
                            *bestUnit = unit;
                            *bestX = targetX;
                            *bestY = targetY;
                        }
                    } else {
                        if (score < *bestScore) {
                            *bestScore = score;
                            *bestUnit = unit;
                            *bestX = targetX;
                            *bestY = targetY;
                        }
                    }
                }
            }
        }
    }
}



// Algorithme MinMax pour évaluer le meilleur coup possible.
int minMax(int depth, bool isMaximizingPlayer, int aiTeam, int currentPlayer) {
    // Si la profondeur maximale est atteinte, évalue la position du plateau
    if (depth == MAX_DEPTH) {
        return evaluatePosition(aiTeam);
    }

    int bestScore = isMaximizingPlayer ? INT_MIN : INT_MAX;
    Unit *bestUnit = NULL;
    int bestX = -1, bestY = -1;

    // Parcourt toutes les unités de l'équipe actuelle
    for (int i = 0; i < NUM_UNITS; i++) {
        Unit *unit = &units[isMaximizingPlayer ? aiTeam : currentPlayer][i];
        if (unit->health > 0 && !unit->tired) {
            evaluateAllMoves(unit, aiTeam, currentPlayer, isMaximizingPlayer, depth, &bestScore, &bestUnit, &bestX, &bestY);
        }
    }

    return bestScore;
}

// Évalue un déplacement spécifique en termes de score.
int evaluateMove(int depth, bool isMaximizingPlayer, int aiTeam, int currentPlayer, Unit *unit, int targetX, int targetY) {
    int originalX = unit->posX;
    int originalY = unit->posY;

    // Déplace temporairement l'unité vers les coordonnées cibles
    unit->posX = targetX;
    unit->posY = targetY;

    // Évalue le score du plateau après ce déplacement
    int score = minMax(depth + 1, !isMaximizingPlayer, aiTeam, currentPlayer);

    // Remet l'unité à sa position d'origine
    unit->posX = originalX;
    unit->posY = originalY;

    return score;
}

// Évalue la position actuelle du plateau pour donner un score.

int evaluatePosition(int aiTeam) {
    int enemyTeam = 1 - aiTeam; // L'équipe ennemie
    int score = 0;

    // Critères de notation basés sur les unités vivantes et leurs points de vie
    score += countLivingUnits(aiTeam) * 1000;
    score += totalHealthPoints(aiTeam) * 20;
    score -= countLivingUnits(enemyTeam) * 500;
    score -= totalHealthPoints(enemyTeam) * 10;
    score -= distanceBetweenUnits(aiTeam); 
    score -= distanceToEnemyUnits(aiTeam, enemyTeam); 

    return score;
}

// Compte le nombre d'unités vivantes dans une équipe.
int countLivingUnits(int team) {
    int count = 0;
    for (int i = 0; i < NUM_UNITS; i++) {
        if (units[team][i].health > 0) {
            count++;
        }
    }
    return count;
}

// Calcule le total des points de vie pour une équipe.
int totalHealthPoints(int team) {
    int total = 0;
    for (int i = 0; i < NUM_UNITS; i++) {
        total += units[team][i].health;
    }
    return total;
}

// Calcule la distance totale entre les unités d'une même équipe.
int distanceBetweenUnits(int team) {
    int totalDistance = 0;
    int unitCount = 0;

    for (int i = 0; i < NUM_UNITS; i++) {
        if (units[team][i].health > 0) {
            unitCount++;
            for (int j = i + 1; j < NUM_UNITS; j++) {
                if (units[team][j].health > 0) {
                    totalDistance += abs(units[team][i].posX - units[team][j].posX) + abs(units[team][i].posY - units[team][j].posY);
                }
            }
        }
    }

    return totalDistance;
}

// Calcule la distance totale entre les unités de l'IA et celles de l'ennemi.
int distanceToEnemyUnits(int aiTeam, int enemyTeam) {
    int totalDistance = 0;
    int unitCount = 0;
    for (int i = 0; i < NUM_UNITS; i++) {
        if (units[aiTeam][i].health > 0) {
            unitCount++;
            for (int j = 0; j < NUM_UNITS; j++) {
                if (units[enemyTeam][j].health > 0) {
                    totalDistance += abs(units[aiTeam][i].posX - units[enemyTeam][j].posX) + abs(units[aiTeam][i].posY - units[enemyTeam][j].posY);
                }
            }
        }
    }

    return totalDistance;
}




// Fonction pour gérer le tour d'un joueur humain.
void playerTurn(int currentPlayer) {
    char unitName;
    int targetX, targetY;

    // Vérifie si toutes les unités de l'équipe sont fatiguées
    isAllTeamTired(currentPlayer);

    // Demande au joueur de sélectionner une unité
    printf("Player %d, select a unit ", currentPlayer + 1);
    printf(currentPlayer == 0 ? "(A, B, C, D):" : "(W, X, Y, Z):");
    scanf(" %c", &unitName);

    // Vérifie si l'unité choisie est fatiguée, sinon demande une autre unité
    while (isTired(currentPlayer, unitName)) {
        printf("The unit you choose is tired...\n");
        printf("Player %d, select a unit (A-D for Player 1, W-Z for Player 2): ", currentPlayer + 1);
        scanf(" %c", &unitName);
    }

    // Demande au joueur les coordonnées cibles pour le déplacement
    printf("    Enter new x coordinates: ");
    scanf("%d", &targetX);
    printf("    Enter new y coordinates: ");
    scanf("%d", &targetY);

    // Vérifie si le déplacement est valide, sinon demande de nouvelles coordonnées
    while (!moveUnit(currentPlayer, unitName, targetX, targetY)) {
        printf("Invalid move!\n");
        printf("Enter new x coordinates: ");
        scanf("%d", &targetX);
        printf("Enter new y coordinates: ");
        scanf("%d", &targetY);
    }

    // Affiche l'état actuel du plateau après le déplacement
    printf("\n-----------------------------------------\n");
    printBoard();

    // Vérifie si une unité ennemie est à portée d'attaque
    if (ennemyInRange(currentPlayer, unitName)) {
        int attackChoice;
        printf("Do you want to attack? (1 = Yes, 0 = No): ");
        scanf("%d", &attackChoice);

        // Si le joueur choisit d'attaquer, demande de sélectionner une cible
        if (attackChoice == 1) {
            char targetName;
            printf("Select a unit to attack (unit name): "); // choix de l'unité à attaquer
            scanf(" %c", &targetName);

            // Tente d'attaquer l'unité cible
            if (attackUnit(currentPlayer, unitName, targetName)) {
                printf("\nAttack successful!\n");
            } else {
                printf("\nAttack failed!\n");
            }
        }
        printf("\n-----------------------------------------\n");
    } else {
        printf("Too far to attack... \n");
        printf("\n-----------------------------------------\n");
    }
}

// Fonction pour annoncer l'équipe gagnante.
void announceWinner() {
    int winner = determineWinner();
    if (winner == 1) {
        printf("Player 1 wins!\n");
    } else if (winner == 2) {
        printf("Player 2 wins!\n");
    } else {
        printf("It's a tie!\n");
    }
    printf("\n-----------------------------------------\n");
}