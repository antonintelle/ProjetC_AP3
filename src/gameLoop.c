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

/**
 * Fonction principale pour le tour de l'IA.
 * @param aiTeam L'équipe de l'IA 
 */
void aiTurn(int aiTeam, Unit *units[NUM_TEAMS][NUM_UNITS]) {
    int bestScore = INT_MIN; 
    Unit *bestUnit = NULL; 
    int bestX = -1, bestY = -1; 

    // Parcourt toutes les unités de l'équipe de l'IA
    for (int i = 0; i < NUM_UNITS; i++) {
        // Vérifie si l'unité n'est pas fatiguée
        if (units[aiTeam][i]->health > 0 && !units[aiTeam][i]->tired) {
            Unit *unit = units[aiTeam][i];
            evaluateAllMoves(unit, aiTeam, 1 - aiTeam, true, 0, &bestScore, &bestUnit, &bestX, &bestY, units);
        }
    }

    // Déplace la meilleure unité vers la meilleure position trouvée
    if (bestUnit != NULL) {
        moveUnit(aiTeam, bestUnit->name, bestX, bestY, units);
        printf("AI moved unit %c to (%d, %d) with score %d\n", bestUnit->name, bestX, bestY, bestScore);
    } else {
        printf("AI could not find a valid move.\n");
    }
}

/**
 * Fonction pour parcourir tous les mouvements possibles d'une unité et évaluer leurs scores.
 * @param unit L'unité à déplacer
 * @param aiTeam L'équipe de l'IA
 * @param currentPlayer L'équipe du joueur actuel
 * @param isMaximizingPlayer Indique si c'est le tour du joueur maximisant
 * @param depth Profondeur actuelle de la recherche
 * @param bestScore Pointeur vers le meilleur score actuel
 * @param bestUnit Pointeur vers la meilleure unité actuelle
 * @param bestX Pointeur vers la meilleure coordonnée X actuelle
 * @param bestY Pointeur vers la meilleure coordonnée Y actuelle
 */
void evaluateAllMoves(Unit *unit, int aiTeam, int currentPlayer, bool isMaximizingPlayer, int depth, int *bestScore, Unit **bestUnit, int *bestX, int *bestY, Unit *units[NUM_TEAMS][NUM_UNITS]) {
    // Parcourt tous les déplacements possibles de l'unité
    for (int dx = -unit->movement; dx <= unit->movement; dx++) {
        for (int dy = -unit->movement; dy <= unit->movement; dy++) {

            // Vérifie si le déplacement est dans la portée de mouvement de l'unité
            if (abs(dx) + abs(dy) <= unit->movement) {
                int targetX = unit->posX + dx;
                int targetY = unit->posY + dy;
                // Vérifie si le déplacement est valide dans les limites de la grille
                if (targetX >= 0 && targetX < COLS && targetY >= 0 && targetY < ROWS) {
                    int score = evaluateMove(depth, !isMaximizingPlayer, aiTeam, currentPlayer, unit, targetX, targetY, units);
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



/**
 * Algorithme MinMax pour évaluer le meilleur coup possible.
 * @param depth Profondeur actuelle de la recherche
 * @param isMaximizingPlayer Indique si c'est le tour du joueur maximisant
 * @param aiTeam L'équipe de l'IA
 * @param currentPlayer L'équipe du joueur actuel
 * @return Le meilleur score évalué
 */
int minMax(int depth, bool isMaximizingPlayer, int aiTeam, int currentPlayer, Unit *units[NUM_TEAMS][NUM_UNITS]) {
    // Si la profondeur maximale est atteinte, évalue la position du plateau
    if (depth == MAX_DEPTH) {
        return evaluatePosition(aiTeam, units);
    }

    int bestScore = isMaximizingPlayer ? INT_MIN : INT_MAX;
    Unit *bestUnit = NULL;
    int bestX = -1, bestY = -1;

    // Parcourt toutes les unités de l'équipe actuelle
    for (int i = 0; i < NUM_UNITS; i++) {
        Unit *unit = units[isMaximizingPlayer ? aiTeam : currentPlayer][i];
        if (unit->health > 0 && !unit->tired) {
            evaluateAllMoves(unit, aiTeam, currentPlayer, isMaximizingPlayer, depth, &bestScore, &bestUnit, &bestX, &bestY, units);
        }
    }

    return bestScore;
}

/**
 * Évalue un déplacement spécifique en termes de score.
 * @param depth Profondeur actuelle de la recherche
 * @param isMaximizingPlayer Indique si c'est le tour du joueur maximisant
 * @param aiTeam L'équipe de l'IA
 * @param currentPlayer L'équipe du joueur actuel
 * @param unit L'unité à déplacer
 * @param targetX Coordonnée X cible
 * @param targetY Coordonnée Y cible
 * @return Le score du mouvement évalué
 */
int evaluateMove(int depth, bool isMaximizingPlayer, int aiTeam, int currentPlayer, Unit *unit, int targetX, int targetY, Unit *units[NUM_TEAMS][NUM_UNITS]) {
    int originalX = unit->posX;
    int originalY = unit->posY;

    // Déplace temporairement l'unité vers les coordonnées cibles
    unit->posX = targetX;
    unit->posY = targetY;

    // Évalue le score du plateau après ce déplacement
    int score = minMax(depth + 1, !isMaximizingPlayer, aiTeam, currentPlayer, units);

    // Remet l'unité à sa position d'origine
    unit->posX = originalX;
    unit->posY = originalY;

    return score;
}

/**
 * Évalue la position actuelle du plateau pour donner un score.
 * @param aiTeam L'équipe de l'IA
 * @return Le score évalué de la position
 */
int evaluatePosition(int aiTeam, Unit *units[NUM_TEAMS][NUM_UNITS]) {
    int enemyTeam = 1 - aiTeam; // L'équipe ennemie
    int score = 0;

    // Critères de notation basés sur les unités vivantes et leurs points de vie
    score += countLivingUnits(aiTeam, units) * 1000;
    score += totalHealthPoints(aiTeam, units) * 20;
    score -= countLivingUnits(enemyTeam, units) * 500;
    score -= totalHealthPoints(enemyTeam, units) * 10;
    score -= distanceBetweenUnits(aiTeam, units); // Pénalité pour distance entre ses propres unités
    score -= distanceToEnemyUnits(aiTeam, enemyTeam, units); // Pénalité pour distance entre ses unités et celles de l'ennemi

    return score;
}

/**
 * Compte le nombre d'unités vivantes dans une équipe.
 * @param team L'équipe concernée
 * @return Le nombre d'unités vivantes
 */
int countLivingUnits(int team, Unit *units[NUM_TEAMS][NUM_UNITS]) {
    int count = 0;
    for (int i = 0; i < NUM_UNITS; i++) {
        if (units[team][i]->health > 0) {
            count++;
        }
    }
    return count;
}

/**
 * Calcule le total des points de vie pour une équipe.
 * @param team L'équipe concernée
 * @return Le total des points de vie
 */
int totalHealthPoints(int team, Unit *units[NUM_TEAMS][NUM_UNITS]) {
    int total = 0;
    for (int i = 0; i < NUM_UNITS; i++) {
        total += units[team][i]->health;
    }
    return total;
}

/**
 * Calcule la distance totale entre les unités d'une même équipe.
 * @param team L'équipe concernée
 * @return La distance totale entre les unités
 */
int distanceBetweenUnits(int team, Unit *units[NUM_TEAMS][NUM_UNITS]) {
    int totalDistance = 0;
    int unitCount = 0;

    for (int i = 0; i < NUM_UNITS; i++) {
        if (units[team][i]->health > 0) {
            unitCount++;
            for (int j = i + 1; j < NUM_UNITS; j++) {
                if (units[team][j]->health > 0) {
                    totalDistance += abs(units[team][i]->posX - units[team][j]->posX) + abs(units[team][i]->posY - units[team][j]->posY);
                }
            }
        }
    }

    return totalDistance;
}

/**
 * Calcule la distance totale entre les unités de l'IA et celles de l'ennemi.
 * @param aiTeam L'équipe de l'IA
 * @param enemyTeam L'équipe ennemie
 * @return La distance totale entre les unités
 */
int distanceToEnemyUnits(int aiTeam, int enemyTeam, Unit *units[NUM_TEAMS][NUM_UNITS]) {
    int totalDistance = 0;
    int unitCount = 0;
    for (int i = 0; i < NUM_UNITS; i++) {
        if (units[aiTeam][i]->health > 0) {
            unitCount++;
            for (int j = 0; j < NUM_UNITS; j++) {
                if (units[enemyTeam][j]->health > 0) {
                    totalDistance += abs(units[aiTeam][i]->posX - units[enemyTeam][j]->posX) + abs(units[aiTeam][i]->posY - units[enemyTeam][j]->posY);
                }
            }
        }
    }

    return totalDistance;
}




/**
 * Fonction pour gérer le tour d'un joueur humain.
 * @param currentPlayer Le joueur actuel (0 ou 1)
 */
void playerTurn(int currentPlayer, Unit *units[NUM_TEAMS][NUM_UNITS]) {
    char unitName;
    int targetX, targetY;

    // Vérifie si toutes les unités de l'équipe sont fatiguées
    isAllTeamTired(currentPlayer, units);

    // Demande au joueur de sélectionner une unité
    printf("Player %d, select a unit ", currentPlayer + 1);
    printf(currentPlayer == 0 ? "(A, B, C, D):" : "(W, X, Y, Z):");
    scanf(" %c", &unitName);

    // Vérifie si l'unité choisie est fatiguée, sinon demande une autre unité
    while (isTired(currentPlayer, unitName, units)) {
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
    while (!moveUnit(currentPlayer, unitName, targetX, targetY, units)) {
        printf("Invalid move!\n");
        printf("Enter new x coordinates: ");
        scanf("%d", &targetX);
        printf("Enter new y coordinates: ");
        scanf("%d", &targetY);
    }

    // Affiche l'état actuel du plateau après le déplacement
    printf("\n-----------------------------------------\n");
    printBoard(units);

    // Vérifie si une unité ennemie est à portée d'attaque
    if (ennemyInRange(currentPlayer, unitName, units)) {
        int attackChoice;
        printf("Do you want to attack? (1 = Yes, 0 = No): ");
        scanf("%d", &attackChoice);

        // Si le joueur choisit d'attaquer, demande de sélectionner une cible
        if (attackChoice == 1) {
            char targetName;
            printf("Select a unit to attack (unit name): "); // choix de l'unité à attaquer
            scanf(" %c", &targetName);

            // Tente d'attaquer l'unité cible
            if (attackUnit(currentPlayer, unitName, targetName, units)) {
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

/**
 * Fonction pour annoncer l'équipe gagnante.
 */
void announceWinner(Unit *units[NUM_TEAMS][NUM_UNITS]) {
    int winner = determineWinner(units);
    if (winner == 1) {
        printf("Player 1 wins!\n");
    } else if (winner == 2) {
        printf("Player 2 wins!\n");
    } else {
        printf("It's a tie!\n");
    }
    printf("\n-----------------------------------------\n");
}