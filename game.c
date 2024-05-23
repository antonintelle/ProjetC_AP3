#include "game.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

Unit units[NUM_TEAMS][NUM_UNITS];

void initializeUnits() {
    // Player 1
    units[0][0] = (Unit){'A', 'A', 0, 3, 1, 3, 2, 3, false, 0, 1};
    units[0][1] = (Unit){'B', 'S', 0, 2, 3, 1, 2, 5, false, 1, 1};
    units[0][2] = (Unit){'C', 'F', 0, 4, 0, 1, 4, 5, false, 0, 3};
    units[0][3] = (Unit){'D', 'S', 0, 2, 3, 1, 2, 5, false, 1, 3};
    // Player 2
    units[1][0] = (Unit){'W', 'S', 1, 2, 3, 1, 2, 5, false, 5, 1};
    units[1][1] = (Unit){'X', 'A', 1, 3, 1, 3, 2, 3, false, 6, 1};
    units[1][2] = (Unit){'Y', 'S', 1, 2, 3, 1, 2, 5, false, 5, 3};
    units[1][3] = (Unit){'Z', 'F', 1, 4, 0, 1, 4, 5, false, 6, 3};
}

void printBoard() {
    // Création du plateau
    char board[ROWS][COLS];
    // Remplissage du palteau avec des caractères vides
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = ' ';
        }
    }
    // Mise à jour des positions des unités
    for (int team = 0; team < NUM_TEAMS; team++) {
        for (int i = 0; i < NUM_UNITS; i++) {
            if (units[team][i].health > 0) {
                board[units[team][i].posY][units[team][i].posX] = units[team][i].name;
            }
        }
    }
    printf("  ");

    // Affichage d'une igne de coordonées
    for (int col = 0; col < COLS; col++) {
        printf("%d ", col);
    }
    printf("\n");

    // Affichage des colonnes
    for (int row = 0; row < ROWS; row++) {
        printf("%c|", 'a' + row);
        for (int col = 0; col < COLS; col++) {
            printf("%c|", board[row][col]);
        }
        printf("\n");
    }
}

bool moveUnit(int team, char name, int targetX, int targetY) {

    for (int i = 0; i < NUM_UNITS; i++) {
        // Séléction de l'unité selon le nom
        if (units[team][i].name == name) {
            // Coordonnées de position de l'unité
            int dx = abs(units[team][i].posX - targetX);
            int dy = abs(units[team][i].posY - targetY);

            if ((dx + dy) <= units[team][i].movement // si la case sélectionnée est à distance de déplacement de l'unité
                && targetX >= 0
                && targetX < COLS
                && targetY >= 0
                && targetY < ROWS) {

                // Vérification qu'il n'y ai pas déjà une unité sur la case sélectionnée
                for (int t = 0; t < NUM_TEAMS; t++) {
                    for (int j = 0; j < NUM_UNITS; j++) {
                        if (units[t][j].posX == targetX && units[t][j].posY == targetY && units[t][j].health > 0) {
                            return false;  // Position occupée, mouvement impossible
                        }
                    }
                }

                // Position libre
                units[team][i].posX = targetX;
                units[team][i].posY = targetY;
                units[team][i].tired = true;
                return true;
            }
        }
    }
    return false;  // Mouvement impossible
}


bool attackUnit(int team, char atkName, char targetName) {
    // Index de l'équipe adverse
    int adv = team == 0 ? 1 : 0;
    Unit *attacker = NULL;
    Unit *defender = NULL;

    // Initialisation des unités attaquante et défenceure
    for (int i = 0; i < NUM_UNITS; i++) {
        if (units[team][i].name == atkName) {
            attacker = &units[team][i];
        }
        if (units[adv][i].name == targetName) {
            defender = &units[adv][i];
        }
    }

    if (attacker
        && defender
        && isInRange(attacker, defender)
        && attacker != defender
        && attacker->team != defender->team) {

        // Les dégâts infligés sont égaux à la puissance du pion qui frappe réduits de la défense du pion qui est frappé
        int damage = attacker->attack - defender->defense;
        damage = damage <= 0 ? damage : 1; // minimum 1
        defender->health -= damage;

        // Contre attaque
        if (defender->health > 0) { // si défenceure est vivante
            damage = defender->attack - attacker->defense;
            damage = damage <= 0 ? damage : 1;
            attacker->health -= damage;
        }

        return true;
    }

    return false;
}

// Vérification que l'unité attaqué est à portée
bool isInRange(Unit *attacker, Unit *defender) {
    int dx = abs(attacker->posX - defender->posX);
    int dy = abs(attacker->posY - defender->posY);
    return (dx + dy) <= attacker->range;
}

// L'unité choisie est-elle fatiguée
bool isTired(char unitName, int team) {

    for (int i = 0; i < NUM_UNITS; i++) {
        if (units[team][i].name == unitName) {
            if (units[team][i].tired){
                return true;
            }
        }
    }
    return false;
}

void isAllTeamTired(int team) {
    int tiredCount = 0;
    // Compte le nombre d'unité fatiguée dans une équipe
    for (int i = 0; i < NUM_UNITS; i++){
        if (units[team][i].tired){
            tiredCount++;
        }
    }
    // Si elles sont toutes fatiguée, remise à 0
    if (tiredCount == NUM_UNITS){
        for (int i = 0; i < NUM_UNITS; i++){
            units[team][i].tired = false;
        }
    }
}

bool checkEndGame() {
    bool teamAlive[2] = {false, false};

    for (int team = 0; team < NUM_TEAMS; team++) {
        for (int i = 0; i < NUM_UNITS; i++) {
            if (units[team][i].health > 0) {
                teamAlive[team] = true;
                break;
            }
        }
    }

    return !(teamAlive[0] && teamAlive[1]);
}

int determineWinner() {
    int healthSum[2] = {0, 0};

    for (int team = 0; team < NUM_TEAMS; team++) {
        for (int i = 0; i < NUM_UNITS; i++) {
            healthSum[team] += units[team][i].health;
        }
    }

    if (healthSum[0] > healthSum[1]) {
        return 1;
    } else if (healthSum[1] > healthSum[0]) {
        return 2;
    }

    return 0;
}

int evaluatePosition(int aiTeam) {
    int score = 0;
    int enemyTeam = 1 - aiTeam;

    for (int i = 0; i < NUM_UNITS; i++) {
        if (units[aiTeam][i].health > 0) {
            score += 1000;
            score += 20 * units[aiTeam][i].health;  // Value health highly
            // New dynamic element: distance to nearest enemy
            int minDist = INT_MAX;
            for (int j = 0; j < NUM_UNITS; j++) {
                if (units[enemyTeam][j].health > 0) {
                    int dist = abs(units[aiTeam][i].posX - units[enemyTeam][j].posX) +
                               abs(units[aiTeam][i].posY - units[enemyTeam][j].posY);
                    if (dist < minDist) minDist = dist;
                }
            }
            score -= minDist * 10;  // Less score for being closer to enemies
        }
    }

    return score;
}




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

bool isValidMove(int x, int y) {
    return x >= 0 && x < COLS && y >= 0 && y < ROWS;
}

void playerTurn(int currentPlayer) {
    char unitName;
    int targetX, targetY;

    isAllTeamTired(currentPlayer);

    printf("Player %d, select a unit ", currentPlayer + 1);
    printf(currentPlayer == 0 ? "(A, B, C, D):" : "(W, X, Y, Z):");
    scanf(" %c", &unitName);

    while(isTired(unitName, currentPlayer)){
        printf("The unit you choose is tired...\n");
        printf("Player %d, select a unit (A-D for Player 1, W-Z for Player 2): ", currentPlayer + 1);
        scanf(" %c", &unitName);
    }

    printf("Enter new x coordinates: ");
    scanf("%d", &targetX);
    printf("Enter new y coordinates: ");
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

    int attackChoice;
    printf("Do you want to attack? (1 = Yes, 0 = No): ");
    scanf("%d", &attackChoice);

    if (attackChoice == 1) {
        char targetName;
        printf("Select a unit to attack (unit name): ");
        scanf(" %c", &targetName);

        if (attackUnit(currentPlayer, unitName, targetName)) {
            printf("Attack successful!\n");
        } else {
            printf("Attack failed!\n");
        }
    }
    printf("\n-----------------------------------------\n");
}

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

void startSinglePlayerGame() {
    initializeUnits();
    int currentPlayer = 0;

    printf("Starting a single player game against AI.\n");
    printf("\n-----------------------------------------\n");

    while (!checkEndGame()) {
        printBoard();
        if (currentPlayer == 0) {
            playerTurn(currentPlayer);
        } else {
            aiTurn(currentPlayer);
        }
        currentPlayer = 1 - currentPlayer;
    }

    announceWinner();
}

void startTwoPlayerGame() {
    initializeUnits();
    int currentPlayer = 0;

    printf("Starting a two player game.\n");
    printf("\n-----------------------------------------\n");

    while (!checkEndGame()) {
        printBoard();
        playerTurn(currentPlayer);
        currentPlayer = 1 - currentPlayer;
    }

    announceWinner();
}

void startAIVsAITest() {
    initializeUnits();
    int currentPlayer = 0;

    printf("Starting AI vs. AI test.\n");
    printf("\n-----------------------------------------\n");

    while (!checkEndGame()) {
        printBoard();
        aiTurn(currentPlayer);
        currentPlayer = 1 - currentPlayer;
    }

    announceWinner();
}

void showOptions() {
    int option;
    printf("Options :\n");
    printf("1. Changer La Taille de la grille(not implemented)\n");
    printf("2. IA Strategie (not implemented)\n");
    printf("Entrez une option: ");
    scanf("%d", &option);

    switch (option) {
        case 1:
            // Placeholder for grid size change
            printf("Grid size change not implemented.\n");
            break;
        case 2:
            // Placeholder for AI strategy settings
            printf("AI strategy change not implemented.\n");
            break;
        default:
            printf("Invalid option.\n");
            break;
    }
}

void showAbout() {
    printf("About the Game:\n");
    printf("Developed by: Your Team Name\n");
    printf("Version: 1.0\n");
    printf("For more information, visit [your-website.com]\n");
}
