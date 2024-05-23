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
bool isInRange(Unit *attacker, Unit *defender);
void printBoard();

bool moveUnit(int team, char name, int targetX, int targetY);
bool attackUnit(int team, char atkName, char targetName);

bool checkEndGame();
int determineWinner();
bool isTired(char unitName, int team);
void isAllTeamTired(int team);

void startSinglePlayerGame();
void startTwoPlayerGame();
void startAIVsAITest();
void announceWinner();
void showOptions();
void showAbout();

void aiTurn(int aiPlayer);
void playerTurn(int currentPlayer);

int evaluatePosition(int aiTeam);
bool isValidMove(int x, int y);