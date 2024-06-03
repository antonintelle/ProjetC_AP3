#include "h/gameMotor.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

Unit units[NUM_TEAMS][NUM_UNITS];

// Initialisation des unités (solats, archers, filous)
void initializeUnits() {
    // Player 1
    units[0][0] = (Unit){'A', 'A', 0, 3, 1, 3, 2, 3, false, 0, 1};
    units[0][1] = (Unit){'B', 'S', 0, 2, 3, 1, 2, 5, false, 1, 1};
    units[0][2] = (Unit){'C', 'F', 0, 4, 0, 1, 4, 5, false, 0, ROWS-2};
    units[0][3] = (Unit){'D', 'S', 0, 2, 3, 1, 2, 5, false, 1, ROWS-2};
    // Player 2
    units[1][0] = (Unit){'W', 'S', 1, 2, 3, 1, 2, 5, false, COLS-2, 1};
    units[1][1] = (Unit){'X', 'A', 1, 3, 1, 3, 2, 3, false, COLS-1, 1};
    units[1][2] = (Unit){'Y', 'S', 1, 2, 3, 1, 2, 5, false, COLS-2, ROWS-2};
    units[1][3] = (Unit){'Z', 'F', 1, 4, 0, 1, 4, 5, false, COLS-1, ROWS-2};
}

// Affiche la grille de jeu avec les unités
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

// mouvement d'un unité vers une case cible
bool moveUnit(int team, char unitName, int targetX, int targetY) {

    for (int i = 0; i < NUM_UNITS; i++) {
        // Séléction de l'unité selon le nom
        if (units[team][i].name == unitName) {
            // Distance en x et y de la case cible
            int dx = abs(units[team][i].posX - targetX);
            int dy = abs(units[team][i].posY - targetY);

            if ((dx + dy) <= units[team][i].movement // si la case sélectionnée est à distance de déplacement de l'unité
                && targetX >= 0
                && targetX < COLS
                && targetY >= 0
                && targetY < ROWS) {

                // Vérification qu'il n'y ai pas déjà une unité sur la case sélectionnée
                for (int t = 0; t < NUM_TEAMS; t++) {
                    for (int u = 0; u < NUM_UNITS; u++) {

                        if (units[t][u].posX == targetX
                        && units[t][u].posY == targetY
                        && units[t][u].health > 0
                        && units[t][u].name != unitName) {
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

// Attaque d'une unité vers une unité ennemie
bool attackUnit(int team, char atkName, char targetName) {
    // Index de l'équipe adverse
    int adv = 1 - team;
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

    if (isInRange(attacker, defender)) {

        // Les dégâts infligés sont égaux à la puissance du pion qui frappe réduits de la défense du pion qui est frappé
        int damage = attacker->attack - defender->defense;
        damage = (damage <= 0) ? 1 : damage; // minimum 1
        defender->health -= damage;

        // Contre attaque
        if (defender->health > 0
            && isInRange(defender, attacker)) { // si défenceure est vivante
            damage = defender->attack - attacker->defense;
            damage = (damage <= 0) ? 1 : damage;
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

// Vérifie si une unité ennemie est à portée
bool ennemyInRange(int team, char atkName){
    int adv = 1 - team;
    Unit *attacker = NULL;

    // initialisation de l'unités attaquante
    for (int i = 0; i < NUM_UNITS; i++) {
        if (units[team][i].name == atkName) {
            attacker = &units[team][i];
        }
    }
    // si au moins une unité est à portée
    for (int i = 0; i < NUM_UNITS; i++) {
        if (isInRange(attacker, &units[adv][i])){
            return true;
        }
    }

    return false;
}

// L'unité choisie est-elle fatiguée
bool isTired(int team, char unitName) {

    for (int i = 0; i < NUM_UNITS; i++) {
        if (units[team][i].name == unitName) {
            if (units[team][i].tired){
                return true;
            }
        }
    }
    return false;
}

// Fonction pour la remise à 0 lorsque toutes les unités sont fatiguées
void isAllTeamTired(int team) {
    int tiredCount = 0;
    int liveCount = 0;
    // Compte le nombre d'unité vivante et fatiguée dans une équipe
    for (int i = 0; i < NUM_UNITS; i++){
        if (units[team][i].health > 0) {
            liveCount++;

            if (units[team][i].tired){
                tiredCount++;
            }
        }
    }
    // Si toutes les unités vivantes sont fatiguée, remise à 0 pour elles
    if (tiredCount == liveCount){
        for (int i = 0; i < NUM_UNITS; i++){
            units[team][i].tired = false;
        }
    }
}

// Vérifie si une équipe a perdu (lorsque toutes ses unités sont mortes ou après 100 coups)
bool checkEndGame(int roundCount) {
    bool teamAlive[2] = {false, false};

    if (roundCount > 100){
        return true;
    }

    for (int team = 0; team < NUM_TEAMS; team++) {
        for (int i = 0; i < NUM_UNITS; i++) {
            if (units[team][i].health > 0) {
                teamAlive[team] = true; // si une unité est vivante l'équipe n'a pas encore perdu
                break;
            }
        }
    }
    return !(teamAlive[0] && teamAlive[1]); // renvoie false uniquement si les deux équipe ont encore des unités vivantes
}

// Détermine l'équipe vainqueur
int determineWinner() {
    int healthSum[2] = {0, 0};

    for (int team = 0; team < NUM_TEAMS; team++) {
        for (int i = 0; i < NUM_UNITS; i++) {
            healthSum[team] += units[team][i].health;
        }
    }

    // renvoie un chiffre selon l'équipe qui le plus de points de vie
    if (healthSum[0] > healthSum[1]) {
        return 1;
    } else if (healthSum[1] > healthSum[0]) {
        return 2;
    }
    if (healthSum[0] == healthSum[1]) {
        return 2;
    }
    return 0;
}

