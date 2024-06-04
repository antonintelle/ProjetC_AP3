#ifndef GAMEMOTOR_H
#define GAMEMOTOR_H

#include "C:/raylib/raylib/src/raylib.h"
#include "C:/raylib/raylib/src/raymath.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 5
#define COLS 7
#define NUM_UNITS 4
#define NUM_TEAMS 2

typedef enum GameScreen
{
    TitleScreen = 0,
    OnePlayer,
    TwoPlayer,
    IATest,
    Options,
    Credits,
    Stop
} GameScreen;

typedef enum State // Etat actuel du jeu
{
    Selection = 0,
    Selectionne,
    Attaque
} State;

typedef struct Button
{
    Rectangle Rec;
    Color Col;

} Button;

typedef struct Cell
{
    bool isCliquable;
    Rectangle Rec;
    Color color;
} Cell;


typedef struct Unit {
    char name;
    char type;
    int team ;
    int attack;
    int defense;
    int range;
    int movement;
    int health;
    int maxHealth;
    bool tired;
    int posX;
    int posY;
    bool selected;
} Unit;

extern Unit units[NUM_TEAMS][NUM_UNITS];

void initializeUnits(Unit *units[NUM_TEAMS][NUM_UNITS], int rows, int cols);
void printBoard(Unit *units[NUM_TEAMS][NUM_UNITS]);

bool isInRange(Unit *attacker, Unit *defender);
bool ennemyInRange(int team, char atkName, Unit *units[NUM_TEAMS][NUM_UNITS]);

bool moveUnit(int team, char unitName, int targetX, int targetY, Unit *units[NUM_TEAMS][NUM_UNITS]);
bool canMoveUnit(int team, char unitName, int targetX, int targetY, Unit *units[NUM_TEAMS][NUM_UNITS]);
bool attackUnit(int team, char atkName, char targetName, Unit *units[NUM_TEAMS][NUM_UNITS]);

bool isTired(int team, char unitName, Unit *units[NUM_TEAMS][NUM_UNITS]);
void isAllTeamTired(int team, Unit *units[NUM_TEAMS][NUM_UNITS]);

bool checkEndGame(int roundCount, Unit *units[NUM_TEAMS][NUM_UNITS]);
int determineWinner(Unit *units[NUM_TEAMS][NUM_UNITS]);

#endif //GAMEMOTOR_H