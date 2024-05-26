#include "gameLoop.h"
#include "gameMotor.h"
#include <stdio.h>

//
void aiTurn(int aiTeam) {
    int bestScore = -10000;
    char bestUnit = ' ';
    int bestX = -1, bestY = -1;

    // Iterate over all units in the AI team
    for (int i = 0; i < NUM_UNITS; i++) {
        if (units[aiTeam][i].health > 0 && !units[aiTeam][i].tired) {
            Unit *unit = &units[aiTeam][i];
            int originalX = unit->posX;
            int originalY = unit->posY;

            // Iterate over all possible moves within the unit's movement range
            for (int dx = -unit->movement; dx <= unit->movement; dx++) {
                for (int dy = -unit->movement; dy <= unit->movement; dy++) {
                    int targetX = unit->posX + dx;
                    int targetY = unit->posY + dy;
                    if (abs(dx) + abs(dy) <= unit->movement && targetX >= 0 && targetX < COLS && targetY >= 0 && targetY < ROWS) {
                        // Temporarily move the unit to the target position
                        unit->posX = targetX;
                        unit->posY = targetY;
                        int score = evaluatePosition(aiTeam);
                        // Move the unit back to its original position
                        unit->posX = originalX;
                        unit->posY = originalY;

                        // If the new score is better, update the best move
                        if (score > bestScore) {
                            bestScore = score;
                            bestUnit = unit->name;
                            bestX = targetX;
                            bestY = targetY;
                        }
                    }
                }
            }
        }
    }

    // Execute the best move found
    if (bestUnit != ' ') {
        moveUnit(aiTeam, bestUnit, bestX, bestY);
        printf("AI moved unit %c to (%d, %d) with score %d\n", bestUnit, bestX, bestY, bestScore);
    } else {
        printf("AI did not find a valid move.\n");
    }

    // Check if the AI can attack after moving
    for (int i = 0; i < NUM_UNITS; i++) {
        if (units[1 - aiTeam][i].health > 0) {
            Unit *attacker = NULL;
            for (int j = 0; j < NUM_UNITS; j++) {
                if (units[aiTeam][j].name == bestUnit) {
                    attacker = &units[aiTeam][j];
                    break;
                }
            }
            if (attacker && isInRange(attacker, &units[1 - aiTeam][i])) {
                attackUnit(aiTeam, bestUnit, units[1 - aiTeam][i].name);
                printf("AI unit %c attacked enemy unit %c\n", bestUnit, units[1 - aiTeam][i].name);
                break;
            }
        }
    }
}

// Tour d'un joueur
void playerTurn(int currentPlayer) {
    char unitName;
    int targetX, targetY;

    isAllTeamTired(currentPlayer);

    printf("Player %d, select a unit ", currentPlayer + 1);
    printf(currentPlayer == 0 ? "(A, B, C, D):" : "(W, X, Y, Z):");
    scanf(" %c", &unitName);

    // choix de l'unité, boucle si fatiguée
    while(isTired(currentPlayer, unitName)){
        printf("The unit you choose is tired...\n");
        printf("Player %d, select a unit (A-D for Player 1, W-Z for Player 2): ", currentPlayer + 1);
        scanf(" %c", &unitName);
    }

    // choix des coordoonées cibles, boucle si mouvement impossible
    printf("    Enter new x coordinates: ");
    scanf("%d", &targetX);
    printf("    Enter new y coordinates: ");
    scanf("%d", &targetY);

    while(!moveUnit(currentPlayer, unitName, targetX, targetY)){
        printf("Invalid move!\n");
        printf("Enter new x coordinates: ");
        scanf("%d", &targetX);
        printf("Enter new y coordinates: ");
        scanf("%d", &targetY);
    }

    printf("\n-----------------------------------------\n");
    printBoard();

    // lance la possibilité d'attaquer si une unité est à portée
    if (ennemyInRange(currentPlayer, unitName)){
        int attackChoice;
        printf("Do you want to attack? (1 = Yes, 0 = No): ");
        scanf("%d", &attackChoice);

        if (attackChoice == 1) {
            char targetName;
            printf("Select a unit to attack (unit name): "); // choix de l'unité à attaquer
            scanf(" %c", &targetName);

            if (attackUnit(currentPlayer, unitName, targetName)) {
                printf("\nAttack successful!\n");
            } else {
                printf("\nAttack failed!\n");
            }
        }
        printf("\n-----------------------------------------\n");
    }
    else {
        printf("Too far to attack... \n");
        printf("\n-----------------------------------------\n");
    }
}

// Message d'annonce de l'équipe vainqueur
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
