#ifndef GAMELOOP_H
#define GAMELOOP_H
#include <limits.h>
#include <stdbool.h>
#include "gameMotor.h"

void aiTurn(int aiPlayer, Unit *units[NUM_TEAMS][NUM_UNITS]);
void playerTurn(int currentPlayer, Unit *units[NUM_TEAMS][NUM_UNITS]);


void announceWinner(Unit *units[NUM_TEAMS][NUM_UNITS]);
int evaluateMove(int depth, bool isMaximizingPlayer, int aiTeam, int currentPlayer, Unit *unit, int targetX, int targetY, Unit *units[NUM_TEAMS][NUM_UNITS]);
int minMax(int depth, bool isMaximizingPlayer, int aiTeam, int currentPlayer, Unit *unit[NUM_TEAMS][NUM_UNITS]);
void evaluateAllMoves(Unit *unit, int aiTeam, int currentPlayer, bool isMaximizingPlayer, int depth, int *bestScore, Unit **bestUnit, int *bestX, int *bestY, Unit *units[NUM_TEAMS][NUM_UNITS]);

int evaluatePosition(int aiTeam, Unit *units[NUM_TEAMS][NUM_UNITS]);
int countLivingUnits(int team, Unit *units[NUM_TEAMS][NUM_UNITS]);
int totalHealthPoints(int team, Unit *units[NUM_TEAMS][NUM_UNITS]);
int distanceBetweenUnits(int team, Unit *units[NUM_TEAMS][NUM_UNITS]);
int distanceToEnemyUnits(int aiTeam, int enemyTeam, Unit *units[NUM_TEAMS][NUM_UNITS]);

#endif //GAMELOOP_H