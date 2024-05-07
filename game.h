#include <stdio.h>
#include <stdbool.h>
#define ROWS 5
#define COLS 7
#define NUM_UNITS 4

typedef struct {
    char type;       // Type de l'unité : 'S' pour soldat, 'A' pour archer, 'F' pour filou
    int health;      // Points de vie
    int attack;      // Puissance d'attaque
    int defense;     // Défense
    int movement;    // Nombre de cases de déplacement possible
    int range;       // Portée d'attaque
    bool isFatigued; // Si l'unité est fatiguée ou non
    int posX;        // Position X sur la grille
    int posY;        // Position Y sur la grille
} Unit;



// Déclarations des fonctions
void initializeUnits(Unit units[2][NUM_UNITS]);
void printBoard(Unit units[2][NUM_UNITS]);
void moveUnit(Unit *unit, int targetX, int targetY);
bool isInRange(Unit *attacker, Unit *defender);
void attackUnit(Unit *attacker, Unit *defender);
void gameLoop(Unit units[2][NUM_UNITS]);