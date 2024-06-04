#include "menu.h"
#include "gameLoop.h"
#include "gameMotor.h"
#include <stdio.h>

void mainMenu() {
    printf("\n\n1. Joueur VS IA\n");
    printf("2. Joueur 1 VS Joueur 2\n");
    printf("3. IA 1 VS IA 2\n");
    printf("4. Options\n");
    printf("5. A propos\n");
    printf("6. Quitter\n");
}

void startSinglePlayerGame() {
    int roundCount = 0;
    initializeUnits();
    int currentPlayer = 0;

    printf("Starting a single player game against AI.\n");
    printf("\n-----------------------------------------\n");

    while (!checkEndGame(roundCount)) {
        printBoard();
        if (currentPlayer == 0) {
            playerTurn(currentPlayer);
        } else {
            aiTurn(currentPlayer);
        }
        roundCount ++;
        currentPlayer = 1 - currentPlayer;
    }

    announceWinner();
}

void startTwoPlayerGame() {
    int roundCount = 0;
    initializeUnits();
    int currentPlayer = 0;

    printf("Starting a two player game.\n");
    printf("\n-----------------------------------------\n");

    while (!checkEndGame(roundCount)) {
        printBoard();
        playerTurn(currentPlayer);
        roundCount++;
        currentPlayer = 1 - currentPlayer;
    }

    announceWinner();
}

void startAIVsAITest() {
    int roundCount = 0;
    initializeUnits();
    int currentPlayer = 0;

    printf("Starting AI vs. AI test.\n");
    printf("\n-----------------------------------------\n");

    while (!checkEndGame(roundCount)) {
        printBoard();
        aiTurn(currentPlayer);
        roundCount++;
        currentPlayer = 1 - currentPlayer;
    }

    announceWinner();
}

void showOptions() {
    int option;
    printf("\nOptions :\n");
    printf("1. Changer La Taille de la grille(not implemented)\n");
    printf("2. IA Strategie (not implemented)\n");
    printf("3. Quitter\n");
    printf("\tEntrez une option: ");
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
        case 3:
            break;
        default:
            printf("Invalid option.\n");
            break;
    }
}

void showAbout() {
    printf("\nAbout the Game:\n");
    printf("Developed by: Antonin Telle, Benjamin Boudry, Imen Mlika, Lucas Gimbert\n");
    printf("Version: 1.0\n");
    printf("Pour plus d'informations, visitez notre site: [website.com]\n");
}
