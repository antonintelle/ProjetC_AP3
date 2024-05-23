#include "gameLoop.h"
#include "gameMotor.h"
#include <stdio.h>

//
void aiTurn(int aiTeam) {
    int bestScore = INT_MIN;
    Move possibleMoves[100];
    int numPossibleMoves = 0;

    // Iterate over all units in the AI team
    for (int i = 0; i < NUM_UNITS; i++) {
        Unit *unit = &units[aiTeam][i];
        if (unit->health > 0 && !unit->tired) {
            int originalX = unit->posX;
            int originalY = unit->posY;

            // Explore all possible moves within the movement range
            for (int dx = -unit->movement; dx <= unit->movement; dx++) {
                for (int dy = -unit->movement; dy <= unit->movement; dy++) {
                    int targetX = originalX + dx;
                    int targetY = originalY + dy;
                    if (isValidMove(targetX, targetY) && abs(dx) + abs(dy) <= unit->movement) {
                        unit->posX = targetX; // Temp move for scoring
                        unit->posY = targetY;
                        int score = evaluatePosition(aiTeam);

                        unit->posX = originalX; // Reset position
                        unit->posY = originalY;

                        // Collect all moves with the best score
                        if (score > bestScore) {
                            bestScore = score;
                            numPossibleMoves = 0;  // Reset the array for best moves
                        }
                        if (score >= bestScore) {
                            if (numPossibleMoves < 100) {
                                possibleMoves[numPossibleMoves++] = (Move){unit->name, targetX, targetY};
                            }
                        }
                    }
                }
            }
        }
    }

    // Randomly choose from the best moves, ensuring different outcomes
    if (numPossibleMoves > 0) {
        int chosenIndex = rand() % numPossibleMoves;
        Move chosenMove = possibleMoves[chosenIndex];
        moveUnit(aiTeam, chosenMove.unitName, chosenMove.x, chosenMove.y);
        printf("AI moved unit %c to (%d, %d) with score %d\n", chosenMove.unitName, chosenMove.x, chosenMove.y, bestScore);
    } else {
        printf("No valid moves available for AI.\n");
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
