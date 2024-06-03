#include <stdio.h>
#include <stdlib.h>
#include "h/menu.h"

int main() {
    int choice;
    while (1) {
        mainMenu();
        printf("\tEntrez une option: ");
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
                printf("Exiting game...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again...\n");
                break;
        }
    }
}
