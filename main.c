#include <stdio.h>
#include <stdlib.h>
#include "game.h"

void main_menu() {
    printf("1. Start Single Player Game\n");
    printf("2. Start Two Player Game\n");
    printf("3. Start AI vs AI Test\n");
    printf("4. Options\n");
    printf("5. About\n");
    printf("6. Quit\n");
}

int main() {
    int choice;
    while (1) {
        main_menu();
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                startSinglePlayerGame();
                break;
            case 2:
                startTwoPlayerGame();
                break;
            case 3:
                startAIVsAITest();
                break;
            case 4:
                showOptions();
                break;
            case 5:
                showAbout();
                break;
            case 6:
                printf("Exiting game.\n");
                exit(0);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}
