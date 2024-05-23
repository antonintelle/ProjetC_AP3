#include <stdio.h>
#include <stdlib.h>
#include "gameMotor.h"

void main_menu() {
    printf("\n\n1. Joueur VS IA\n");
    printf("2. Joueur 1 VS Joueur 2\n");
    printf("3. IA 1 VS IA 2\n");
    printf("4. Options\n");
    printf("5. A propos\n");
    printf("6. Quitter\n");
}

int main() {
    int choice;
    while (1) {
        main_menu();
        printf("\tEntrez an option: ");
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
                break;
            default:
                printf("Invalid choice. Please try again...\n");
                break;
        }
    }
}
