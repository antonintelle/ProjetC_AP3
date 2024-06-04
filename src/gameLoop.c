#include "gameLoop.h"
#include "gameMotor.h"
#include <stdio.h>
#include <stdlib.h>

void aiTurn(int aiTeam, Unit *units[NUM_TEAMS][NUM_UNITS]) {
    int bestScore = 0; //meilleur score
    Unit *bestUnit = NULL; // meilleure unité à déplacer
    int bestX = -1, bestY = -1;
    int adv = 1 - aiTeam;
    int score;
    bool attackWithGoodScore = false;
    char ennemyToAttack;

    isAllTeamTired(aiTeam, units);

    // Parcourt toutes les unités de l'équipe de l'IA
    for (int i = 0; i < NUM_UNITS; i++) {
        // Si l'unité est vivante et pas fatiguée
        if (units[aiTeam][i]->health > 0
        && !units[aiTeam][i]->tired) {

            Unit *unit = units[aiTeam][i];
            int originalX = unit->posX; // Position de l'unité
            int originalY = unit->posY;

            // Parcourt toutes les positions possibles dans la portée de mouvement de l'unité
            for (int dx = -unit->movement; dx <= unit->movement; dx++) {
                for (int dy = -unit->movement; dy <= unit->movement; dy++) {

                    int targetX = unit->posX + dx;
                    int targetY = unit->posY + dy;

                    if(moveUnit(aiTeam, unit->name, targetX, targetY, units)) { // Test de déplacement
                        int bestScoreAttack = 0;
                        int scoreWithoutAttack = evaluateScore(aiTeam, units);

                        for (int ennemyUnit = 0; ennemyUnit < NUM_UNITS; ennemyUnit++) {
                            if (isInRange(unit, units[adv][ennemyUnit])){ // Si un ennemi est à portée on test de l'attaquer

                                int originalEnnemyHealth = units[adv][ennemyUnit]->health;
                                int originalHealth = unit->health;

                                if (attackUnit(aiTeam, unit->name, units[adv][ennemyUnit]->name, units)) {
                                    if (evaluateScore(aiTeam, units) > bestScoreAttack) { // Si l'attaque mène à un meilleur score
                                        attackWithGoodScore = true; // Pour effectuer l'attaque si le score est meilleur que si l'on attaque pas
                                        bestScoreAttack = evaluateScore(aiTeam, units);
                                        ennemyToAttack = units[adv][ennemyUnit]->name;  // Nom de l'ennemi à attaquer pour avoir le meilleur score
                                    }
                                    // Remise à l'origine des vies des unités
                                    units[adv][ennemyUnit]->health = originalEnnemyHealth;
                                    unit->health = originalHealth;
                                }
                            }
                        }
                        if (scoreWithoutAttack > bestScoreAttack){
                            score = scoreWithoutAttack;
                            attackWithGoodScore = false;
                        }
                    } else {
                        score = 0;
                    }

                    unit->posX = originalX; // restaurer le déplacement et l'état fatigué
                    unit->posY = originalY;
                    unit->tired = false;

                    // Met à jour le meilleur score et la meilleure position si nécessaire
                    if (score > bestScore) {
                        bestScore = score;
                        bestUnit = unit;
                        bestX = targetX;
                        bestY = targetY;
                    }
                }
            }
        }
    }
    // effectuer le déplacement si une meilleure unité et position trouvées
    if (bestUnit != NULL) {
        moveUnit(aiTeam, bestUnit->name, bestX, bestY, units);
        printf("AI moved unit %c to (%d, %d) with score %d\n", bestUnit->name, bestX, bestY, bestScore);

        if (attackWithGoodScore) { // Effectuer l'attaque si elle permet un bon score

            if(attackUnit(aiTeam, bestUnit->name, ennemyToAttack, units)){
                printf("AI attack unit %c\n", ennemyToAttack);

            }else {
                printf("Attack failed\n");
            }
        }

    } else {
        printf("AI could not find a valid move.\n");
    }
}

int evaluateScore(int aiTeam, Unit *units[NUM_TEAMS][NUM_UNITS]) {
    int enemyTeam = 1 - aiTeam;
    int score = 0;

    // Critères de notation basés sur les unités vivantes et leurs points de vie
    score += countLivingUnits(aiTeam, units) * 1000;
    score += totalHealthPoints(aiTeam, units) * 1;
    score -= countLivingUnits(enemyTeam, units) * 500;
    score -= totalHealthPoints(enemyTeam, units) * 10;
    score -= distanceBetweenUnits(aiTeam, units); // Pénalité pour distance entre ses propres unités
    score -= distanceToEnemyUnits(aiTeam, enemyTeam, units); // Pénalité pour distance entre ses unités et celles de l'ennemi

    return score;
}


// Compte le nombre d'unités vivantes dans une équipe
int countLivingUnits(int team, Unit *units[NUM_TEAMS][NUM_UNITS]) {
    int count = 0;
    for (int i = 0; i < NUM_UNITS; i++) {
        if (units[team][i]->health > 0) {
            count++;
        }
    }
    return count;
}


// Calcule le total des points de vie pour une équipe
int totalHealthPoints(int team, Unit *units[NUM_TEAMS][NUM_UNITS]) {
    int total = 0;
    for (int i = 0; i < NUM_UNITS; i++) {
        total += units[team][i]->health;
    }
    return total;
}


// Calcule la distance totale entre les unités d'une même équipe
int distanceBetweenUnits(int team, Unit *units[NUM_TEAMS][NUM_UNITS]) {
    int totalDistance = 0;
    int unitCount = 0;

    for (int i = 0; i < NUM_UNITS; i++) {
        if (units[team][i]->health > 0) {

            unitCount++;

            for (int j = i + 1; j < NUM_UNITS; j++) {
                if (units[team][j]->health > 0) {

                    totalDistance +=
                            abs(units[team][i]->posX - units[team][j]->posX)
                            + abs(units[team][i]->posY - units[team][j]->posY);
                }
            }
        }
    }

    return totalDistance;
}

//calcul de la distance totale entre toutes les unités vivantes de l'IA
int distanceToEnemyUnits(int aiTeam, int enemyTeam, Unit *units[NUM_TEAMS][NUM_UNITS]) {
    int totalDistance = 0;
    int unitCount = 0;

    for (int i = 0; i < NUM_UNITS; i++) {
        if (units[aiTeam][i]->health > 0) {

            unitCount++;

            for (int j = 0; j < NUM_UNITS; j++) {
                if (units[enemyTeam][i]->health > 0) {

                    totalDistance +=
                            abs(units[aiTeam][i]->posX - units[enemyTeam][i]->posX)
                            + abs(units[aiTeam][i]->posY - units[enemyTeam][i]->posY);
                }
            }
        }
    }

    return totalDistance;
}
