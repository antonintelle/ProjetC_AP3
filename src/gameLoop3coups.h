#ifndef GAMELOOP_H
#define GAMELOOP_H
#include <limits.h>
#include <stdbool.h>
#include "gameMotor.h"

void aiTurn(int aiPlayer);
void playerTurn(int currentPlayer);


void announceWinner();
int evaluateMove(int depth, bool isMaximizingPlayer, int aiTeam, int currentPlayer, Unit *unit, int targetX, int targetY);
void evaluateAllMoves(Unit *unit, int aiTeam, int currentPlayer, bool isMaximizingPlayer, int depth, int *bestScore, Unit **bestUnit, int *bestX, int *bestY);
int minMax(int depth, bool isMaximizingPlayer, int aiTeam, int currentPlayer);


int evaluatePosition(int aiTeam);
int countLivingUnits(int team);
int totalHealthPoints(int team);
int distanceBetweenUnits(int team);
int distanceToEnemyUnits(int aiTeam, int enemyTeam);

#endif //GAMELOOP_H