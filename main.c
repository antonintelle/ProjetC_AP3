#include "C:/raylib/raylib/src/raylib.h"
#include "C:/raylib/raylib/src/raymath.h"
#include <string.h>
#include "src/gameMotor.h"
#include "src/gameLoop.h"

#define screenWidth 1920
#define screenHeight 1080

void drawUnits(Unit *units[NUM_TEAMS][NUM_UNITS])
{
    for (int team = 0; team < NUM_TEAMS; team++)
    {
        for (int i = 0; i < NUM_UNITS; i++)
        {
            if (units[team][i]->health > 0)
            {
                if (team == 0)
                {
                    if (units[team][i]->tired)
                    {
                        DrawCircle(units[team][i]->posX * screenWidth / COLS + screenWidth / COLS / 2, units[team][i]->posY * screenHeight / ROWS + screenHeight / ROWS / 2 - 5, 60, DARKBLUE);
                    }
                    else
                        DrawCircle(units[team][i]->posX * screenWidth / COLS + screenWidth / COLS / 2, units[team][i]->posY * screenHeight / ROWS + screenHeight / ROWS / 2 - 5, 60, BLUE);
                }
                else
                {
                    if (units[team][i]->tired)
                    {
                        DrawCircle(units[team][i]->posX * screenWidth / COLS + screenWidth / COLS / 2, units[team][i]->posY * screenHeight / ROWS + screenHeight / ROWS / 2 - 5, 60, BROWN);
                    }
                    else
                        DrawCircle(units[team][i]->posX * screenWidth / COLS + screenWidth / COLS / 2, units[team][i]->posY * screenHeight / ROWS + screenHeight / ROWS / 2 - 5, 60, RED);
                }
                char temp[2]; // Liste temporaire
                temp[0] = units[team][i]->type;
                temp[1] = '\0';
                const char *str = temp;
                DrawText(str, units[team][i]->posX * screenWidth / COLS + screenWidth / COLS / 2 - (MeasureText(str, 50) / 2), units[team][i]->posY * screenHeight / ROWS + screenHeight / ROWS / 2 - 25, 50, BLACK);

                // Barre de vie
                DrawRectangle(units[team][i]->posX * screenWidth / COLS + screenWidth / COLS / 2 - 30,
                              units[team][i]->posY * screenHeight / ROWS + screenHeight / ROWS / 2 + 30,
                              60,
                              10, GRAY);
                // printf("%f / %f\n", units[team][i]->health, units[team][i]->maxHealth);
                int healthbar = (int)(60 * (float)(units[team][i]->health) / (float)(units[team][i]->maxHealth));

                DrawRectangle(units[team][i]->posX * screenWidth / COLS + screenWidth / COLS / 2 - 30,
                              units[team][i]->posY * screenHeight / ROWS + screenHeight / ROWS / 2 + 30,
                              (int)(60 * (float)(units[team][i]->health) / (float)(units[team][i]->maxHealth)),
                              10, GREEN);
            }
        }
    }
}

bool moveableUnitInCell(int team, int x, int y, Unit *units[NUM_TEAMS][NUM_UNITS])
{
    for (int i = 0; i < NUM_UNITS; i++)
    {
        if (units[team][i]->posX == x && units[team][i]->posY == y && units[team][i]->health > 0)
        {
            return true;
        }
    }
    return false;
}

// Afficher une grille sans possibilité de jouer
void drawGridIA(int rows, int cols, Unit *units[NUM_TEAMS][NUM_UNITS])
{
    Cell grid[rows][cols];
    for (int row = 0; row < rows; row++) // Pour chaque ligne
    {
        for (int col = 0; col < cols; col++) // Pour chaque colonne
        {
            // Initialisation des cases
            grid[row][col].Rec = (Rectangle){screenWidth / cols * col, screenHeight / rows * row, screenWidth / cols, screenHeight / rows};
            grid[row][col].isCliquable = false;

            if ((row + col) % 2 == 0)
            {
                grid[row][col].color = WHITE;
            }
            else
            {
                grid[row][col].color = BLACK;
            }
            DrawRectangleRec(grid[row][col].Rec, grid[row][col].color);
        }
    }
}

// Afficher une grille avec possibilité de jouer [NEW]
void Grid(int rows,
          int cols,
          Vector2 mousePoint,
          int *turn,
          State *state,
          Unit *selectedUnit,
          int *nbRound,
          Unit *units[NUM_TEAMS][NUM_UNITS])
{
    Cell grid[rows][cols];
    for (int col = 0; col < cols; col++)
    {
        for (int row = 0; row < rows; row++)
        {
            grid[row][col].Rec = (Rectangle){screenWidth / cols * col, screenHeight / rows * row, screenWidth / cols, screenHeight / rows};
            if ((row + col) % 2 == 0)
            {
                grid[row][col].color = WHITE;
            }
            else
            {
                grid[row][col].color = BLACK;
            }
            switch (*state)
            {
            case Selection:
#pragma region Selection d une unité
                for (int i = 0; i < NUM_UNITS; i++)
                {
                    if (units[*turn][i]->posX == col && units[*turn][i]->posY == row && CheckCollisionPointRec(mousePoint, grid[row][col].Rec) && units[*turn][i]->health > 0 && !units[*turn][i]->tired)
                    {
                        grid[row][col].color = GREEN;
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        {
                            *selectedUnit = *units[*turn][i];
                            *state = Selectionne;
                        }
                    }
                }
                break;
#pragma endregion
            // Unité sélectionnée
            case Selectionne:
#pragma region Selection d un mouvement
                if (canMoveUnit(*turn, selectedUnit->name, col, row, units))
                {
                    grid[row][col].color = YELLOW;
                    if (CheckCollisionPointRec(mousePoint, grid[row][col].Rec)) // Si la case est cliquée
                    {
                        grid[row][col].color = ORANGE;
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        {
                            moveUnit(*turn, selectedUnit->name, col, row, units);
                            if (ennemyInRange(*turn, selectedUnit->name, units))
                            {
                                *state = Attaque;
                                printf("Attaque possible\n");
                                for (int i = 0; i < NUM_UNITS; i++)
                                {
                                    if (selectedUnit->name == units[*turn][i]->name)
                                    {
                                        *selectedUnit = *units[*turn][i];
                                    }
                                }
                            }
                            else
                            {
                                *state = Selection;
                                *turn = abs(1 - *turn);
                                *nbRound += 1;
                                printf("Attaque impossible\n");
                            }
                        }
                    }
                }
                break;
#pragma endregion
            // Attaque possible
            case Attaque:
#pragma region Attaque
                for (int i = 0; i < NUM_UNITS; i++)
                {
                    if (col == 3 && row == 1)
                    {
                        int a = 0;
                    }
                    if (units[1 - *turn][i]->posX == col && units[1 - *turn][i]->posY == row)
                        if (isInRange(selectedUnit, units[abs(1 - *turn)][i]))
                        {
                            grid[row][col].color = MAROON;
                            if (CheckCollisionPointRec(mousePoint, grid[row][col].Rec))
                            {
                                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                                {
                                    attackUnit(*turn, selectedUnit->name, units[abs(1 - *turn)][i]->name, units);
                                    *state = Selection;
                                    *turn = abs(1 - *turn);
                                    *nbRound += 1;
                                }
                            }
                        }
                }
                printf("\n");
                break;
#pragma endregion
            }

            DrawRectangleRec(grid[row][col].Rec, grid[row][col].color);
        }
    }
}

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const char *TITLE = "THE GAME";
    int turn = 0;
    State state = Selection;
    Unit *SelectedUnit;
    Unit *units[NUM_TEAMS][NUM_UNITS];

    for (int i = 0; i < NUM_TEAMS; i++)
    {
        for (int j = 0; j < NUM_UNITS; j++)
        {
            units[i][j] = malloc(sizeof(Unit));
            if (units[i][j] == NULL)
            {
                // Handle error
                printf("Failed to allocate memory for unit\n");
                exit(1);
            }
        }
    }
    int nbRound = 0;

    InitWindow(screenWidth, screenHeight, TITLE);

    GameScreen CurrentScreen = TitleScreen;

    Texture2D Background = LoadTexture("img/background.png");

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    Vector2 mousePoint = {0.0f, 0.0f};

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        mousePoint = GetMousePosition();
#pragma region TitleScreen

        Rectangle Rec1vs1 = {screenWidth / 10, (screenHeight / 100) * 30, (screenWidth / 100) * 80, (screenHeight / 10)};
        Button But1vs1 = {Rec1vs1, GRAY};
        Rectangle Rec1vsIA = {screenWidth / 10, (screenHeight / 100) * 40, (screenWidth / 100) * 80, (screenHeight / 10)};
        Button But1vsIA = {Rec1vsIA, BLACK};
        Rectangle RecTestIA = {screenWidth / 10, (screenHeight / 100) * 50, (screenWidth / 100) * 80, (screenHeight / 10)};
        Button ButTestIA = {RecTestIA, GRAY};
        Rectangle RecOptions = {screenWidth / 10, (screenHeight / 100) * 60, (screenWidth / 100) * 80, (screenHeight / 10)};
        Button ButOptions = {RecOptions, BLACK};
        Rectangle RecCredits = {screenWidth / 10, (screenHeight / 100) * 70, (screenWidth / 100) * 80, (screenHeight / 10)};
        Button ButCredits = {RecCredits, GRAY};
        Rectangle RecQuitter = {screenWidth / 10, (screenHeight / 100) * 80, (screenWidth / 100) * 80, (screenHeight / 10)};
        Button ButQuitter = {RecQuitter, BLACK};

#pragma endregion
        switch (CurrentScreen)
        {
        case TitleScreen:

            if (CheckCollisionPointRec(mousePoint, But1vs1.Rec))
            {
                But1vs1.Col = GREEN;
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    CurrentScreen = TwoPlayer;
                    WaitTime(0.2);
                    initializeUnits(units);
                }
            }
            if (CheckCollisionPointRec(mousePoint, But1vsIA.Rec))
            {
                But1vsIA.Col = GREEN;
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    CurrentScreen = OnePlayer;
                    WaitTime(0.2);
                    initializeUnits(units);
                }
            }
            if (CheckCollisionPointRec(mousePoint, ButTestIA.Rec))
            {
                ButTestIA.Col = GREEN;
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    CurrentScreen = IATest;
                    initializeUnits(units);
                }
            }
            if (CheckCollisionPointRec(mousePoint, ButCredits.Rec))
            {
                ButCredits.Col = GREEN;
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    CurrentScreen = Credits;
            }
            if (CheckCollisionPointRec(mousePoint, ButOptions.Rec))
            {
                ButOptions.Col = GREEN;
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    CurrentScreen = Options;
            }
            if (CheckCollisionPointRec(mousePoint, ButQuitter.Rec))
            {
                ButQuitter.Col = GREEN;
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    CloseWindow();
            }
            turn = 0;
            state = Selection;
            nbRound = 0;
            break;

        case OnePlayer:
            isAllTeamTired(turn, units);
            if (turn == 1)
            {
                aiTurn(1, units);
                turn = 0;
            }
            break;

        case TwoPlayer:
            isAllTeamTired(turn, units);
            break;

        case IATest:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                CurrentScreen = TitleScreen;
            }
            break;

        case Options:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                CurrentScreen = TitleScreen;
            }
            break;

        case Credits:
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                CurrentScreen = TitleScreen;
            }
            break;

        default:
            break;
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
        switch (CurrentScreen)
        {
        case TitleScreen:
            DrawTexture(Background, 0, 0, WHITE);
            DrawText(TITLE, screenWidth / 2 - MeasureText(TITLE, 150) / 2, 20, 150, WHITE);
            DrawRectangleRec(But1vs1.Rec, But1vs1.Col);
            DrawText("1vs1", screenWidth / 2 - MeasureText("1vs1", 100) / 2, (screenHeight / 100) * 30, 100, WHITE);
            DrawRectangleRec(But1vsIA.Rec, But1vsIA.Col);
            DrawText("1vsIA", screenWidth / 2 - MeasureText("1vsIA", 100) / 2, (screenHeight / 100) * 40, 100, WHITE);
            DrawRectangleRec(ButTestIA.Rec, ButTestIA.Col);
            DrawText("Test IA", screenWidth / 2 - MeasureText("Test IA", 100) / 2, (screenHeight / 100) * 50, 100, WHITE);
            DrawRectangleRec(ButOptions.Rec, ButOptions.Col);
            DrawText("Options", screenWidth / 2 - MeasureText("Options", 100) / 2, (screenHeight / 100) * 60, 100, WHITE);
            DrawRectangleRec(ButCredits.Rec, ButCredits.Col);
            DrawText("Crédits", screenWidth / 2 - MeasureText("Crédits", 100) / 2, (screenHeight / 100) * 70, 100, WHITE);
            DrawRectangleRec(ButQuitter.Rec, ButQuitter.Col);
            DrawText("Quitter", screenWidth / 2 - MeasureText("Quitter", 100) / 2, (screenHeight / 100) * 80, 100, WHITE);
            break;

        case OnePlayer:
            if (checkEndGame(nbRound, units))
            {
                ClearBackground(BLACK);
                // char *winner = [ "bleu", "rouge" ][determineWinner()];
                // DrawText(strcat("L'équipe ", *winner), screenWidth / 2 - MeasureText(strcat("L'équipe ", *winner), 100) / 2, screenHeight / 2 - 100, 100, WHITE);
                // DrawText("a gagné !", screenWidth / 2 - MeasureText("a gagné !", 100) / 2, screenHeight / 2, 100, WHITE);
            }
            else
            {
                Grid(ROWS, COLS, mousePoint, &turn, &state, SelectedUnit, &nbRound, units);
                // drawGrid(ROWS, COLS, mousePoint, &turn);
                drawUnits(units);
            }
            break;

        case TwoPlayer:
            if (checkEndGame(nbRound, units))
            {
                ClearBackground(BLACK);
                if (determineWinner(units) == 1)
                    DrawText("L'équipe bleu", screenWidth / 2 - MeasureText("L'équipe bleu", 100) / 2, screenHeight / 2 - 100, 100, WHITE);
                else
                    DrawText("L'équipe rouge", screenWidth / 2 - MeasureText("L'équipe rouge", 100) / 2, screenHeight / 2 - 100, 100, WHITE);
                DrawText("a gagné !", screenWidth / 2 - MeasureText("a gagné !", 100) / 2, screenHeight / 2, 100, WHITE);
            }
            else
            {
                Grid(ROWS, COLS, mousePoint, &turn, &state, SelectedUnit, &nbRound, units);
                drawUnits(units);
            }
            break;

        case IATest:
            if (checkEndGame(nbRound, units))
            {
                ClearBackground(BLACK);
                // char *winner = [ "bleu", "rouge" ][determineWinner()];
                // DrawText(strcat("L'équipe ", *winner), screenWidth / 2 - MeasureText(strcat("L'équipe ", *winner), 100) / 2, screenHeight / 2 - 100, 100, WHITE);
                // DrawText("a gagné !", screenWidth / 2 - MeasureText("a gagné !", 100) / 2, screenHeight / 2, 100, WHITE);
            }
            else
            {
                drawGridIA(ROWS, COLS, units);
                drawUnits(units);
            }
            break;

        case Options:
            ClearBackground(BLACK);
            DrawText("Options", screenWidth / 2 - MeasureText("Options", 100) / 2, screenHeight / 2 - 100, 100, WHITE);
            break;

        case Credits:
            ClearBackground(BLACK);
            DrawText("Crédits", screenWidth / 2 - MeasureText("Crédits", 100) / 2, screenHeight / 2 - 250, 100, WHITE);
            DrawText("Développeurs : ", screenWidth / 2 - MeasureText("Développeurs : ", 75) / 2, screenHeight / 2 - 250 + 100, 75, WHITE);
            DrawText("Antonin TELLE", screenWidth / 2 - MeasureText("Antonin TELLE", 50) / 2, screenHeight / 2 - 250 + 175, 50, WHITE);
            DrawText("Imen MLIKA", screenWidth / 2 - MeasureText("Imen MLIKA", 50) / 2, screenHeight / 2 - 250 + 225, 50, WHITE);
            DrawText("Lucas GIMBERT", screenWidth / 2 - MeasureText("Lucas GIMBERT", 50) / 2, screenHeight / 2 - 250 + 275, 50, WHITE);
            DrawText("Benjamin BOUDRY", screenWidth / 2 - MeasureText("Benjamin BOUDRY", 50) / 2, screenHeight / 2 - 250 + 325, 50, WHITE);
            DrawText("Bibliothèque utilisés :", screenWidth / 2 - MeasureText("Bibliothèque utilisés :", 75) / 2, screenHeight / 2 - 250 + 425, 75, WHITE);
            DrawText("Raylib", screenWidth / 2 - MeasureText("Raylib", 50) / 2, screenHeight / 2 + 250, 50, WHITE);
            break;

        default:
            break;
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    for (int i = 0; i < NUM_TEAMS; i++)
    {
        for (int j = 0; j < NUM_UNITS; j++)
        {
            free(units[i][j]);
        }
    }

    return 0;
}