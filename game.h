#include <stdio.h>
#include <stdbool.h>
#define ROWS 5
#define COLS 7
#define NUM_UNITS 4
#define NUM_TEAMS 2
 
typedef struct {
    char name;       // Nom de l'unité sur la grille
    char type;       // Type de l'unité : 'S' pour soldat, 'A' pour archer, 'F' pour filou
    int health;      // Points de vie
    int attack;      // Puissance d'attaque
    int defense;     // Défense
    int movement;    // Nombre de cases de déplacement possible
    int range;       // Portée d'attaque
    bool isTired;    // Si l'unité est fatiguée ou non
    int posX;        // Position X sur la grille
    int posY;        // Position Y sur la grille
} Unit;
 
extern Unit units[NUM_TEAMS][NUM_UNITS];


// Déclarations des unités
void initializeUnits(Unit units[2][NUM_UNITS]);

void printBoard(Unit units[2][NUM_UNITS]);

bool moveUnit(int team, int unitIndex, int targetX, int targetY); // Déplace une unité vers les coordonées cible

bool isInRange(Unit *attacker, Unit *defender); // Vérifie si l'ennemi est atteignable

bool attackUnit(int team, int unitIndex, int targetX, int targetY); // Attaque l'unité qui se trouve aux coordonéées cibles

//void gameLoop

bool checkEndGame(Unit units[2][NUM_UNITS]); // Vérifie les conditions de fin de jeu

int determineWinner(Unit units[2][NUM_UNITS]); //  Détermine le vainqueur
