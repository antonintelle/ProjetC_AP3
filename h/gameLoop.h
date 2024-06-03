#ifndef GAMELOOP_H
#define GAMELOOP_H
#include <limits.h>
#include <stdbool.h>

void aiTurn(int aiPlayer);
void playerTurn(int currentPlayer);
bool isValidMove(int x, int y);
int evaluateScore(int aiTeam);
int traverseUnitsAndMoves(int depth, bool isMaximizingPlayer, int aiTeam, int currentPlayer, int *bestX, int *bestY, bool updateBestMove);

int coupX1(int depth, int aiTeam, int adv);
int evaluateScore(int aiTeam);
int countLivingUnits(int team);
int totalHealthPoints(int team);
int distanceBetweenUnits(int team);
int distanceToEnemyUnits(int aiTeam, int enemyTeam);

#endif //GAMELOOP_H