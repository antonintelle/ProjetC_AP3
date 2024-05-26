#ifndef GAMEMOTOR_H
#define GAMEMOTOR_H

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

typedef struct {
    char name;
    char type;
    int team ;
    int attack;
    int defense;
    int range;
    int movement;
    int health;
    bool tired;
    int posX;
    int posY;
} Unit;

typedef struct {
    char unitName;
    int x;
    int y;
} Move;

extern Unit units[NUM_TEAMS][NUM_UNITS];

void initializeUnits();
void printBoard();

bool isInRange(Unit *attacker, Unit *defender);
bool ennemyInRange(int team, char atkName);

bool moveUnit(int team, char unitName, int targetX, int targetY);
bool attackUnit(int team, char atkName, char targetName);

bool isTired(int team, char unitName);
void isAllTeamTired(int team);

int evaluatePosition(int aiTeam);
bool isValidMove(int x, int y);

bool checkEndGame(int roundCount);
int determineWinner();

#endif //GAMEMOTOR_H