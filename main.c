#include "C:/raylib/raylib/src/raylib.h"
#include "C:/raylib/raylib/src/raymath.h"

typedef enum GameScreen { TitleScreen = 0, OnePlayer, TwoPlayer, IATest, Options, Credits, Stop } GameScreen;

typedef struct Button {
    Rectangle Rec;
    Color Col;
    
} Button;

typedef struct {
    int x;
    int y;
    bool isCliquable;
    Color color;
} Cell;

void DrawGrid(Cell grid[][], int screenWidth, int screenHeight) {
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 7; col++) {
            grid[row][col].x = (screenWidth/7);
            grid[row][col].y = 100 + row * 100;
            grid[row][col].isCliquable = true;
            if (row+col % 2 == 0) {
                grid[row][col].color = WHITE;
            } else {
                grid[row][col].color = BLACK;
            }
        }
    }
}

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    const char* Title = "THE GAME";

    InitWindow(screenWidth, screenHeight, Title);

    GameScreen CurrentScreen = TitleScreen; 

    Texture2D Background = LoadTexture("img/Background.png");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    Vector2 mousePoint = {0.0f, 0.0f};

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        mousePoint = GetMousePosition();
        #pragma region TitleScreen

        Rectangle Rec1vs1 = { screenWidth/10, (screenHeight/100)*30, (screenWidth/100)*80, (screenHeight/10) };
        Button But1vs1 = {Rec1vs1, GRAY};
        Rectangle Rec1vsIA = { screenWidth/10, (screenHeight/100)*40, (screenWidth/100)*80, (screenHeight/10) };
        Button But1vsIA = {Rec1vsIA, BLACK};
        Rectangle RecTestIA = { screenWidth/10, (screenHeight/100)*50, (screenWidth/100)*80, (screenHeight/10) };
        Button ButTestIA = {RecTestIA, GRAY};
        Rectangle RecOptions = { screenWidth/10, (screenHeight/100)*60, (screenWidth/100)*80, (screenHeight/10) };
        Button ButOptions = {RecOptions, BLACK};
        Rectangle RecCredits = { screenWidth/10, (screenHeight/100)*70, (screenWidth/100)*80, (screenHeight/10) };
        Button ButCredits = {RecCredits, GRAY};
        Rectangle RecQuitter = { screenWidth/10, (screenHeight/100)*80, (screenWidth/100)*80, (screenHeight/10) };
        Button ButQuitter = {RecQuitter, BLACK};

        #pragma endregion
        switch (CurrentScreen)
        {
        case TitleScreen:
            if (CheckCollisionPointRec(mousePoint, But1vs1.Rec)) {
                But1vs1.Col = GREEN;
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
                    CurrentScreen = OnePlayer;
            }
            if (CheckCollisionPointRec(mousePoint, But1vsIA.Rec)) {
                But1vsIA.Col = GREEN;
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
                    CurrentScreen = TwoPlayer;
            }
            if (CheckCollisionPointRec(mousePoint, ButTestIA.Rec)) {
                ButTestIA.Col = GREEN;
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
                    CurrentScreen = IATest;
            }
            if (CheckCollisionPointRec(mousePoint, ButCredits.Rec)) {
                ButCredits.Col = GREEN;
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
                    CurrentScreen = Credits;
            }
            if (CheckCollisionPointRec(mousePoint, ButOptions.Rec)) {
                ButOptions.Col = GREEN;
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
                    CurrentScreen = Options;
            }
            if (CheckCollisionPointRec(mousePoint, ButQuitter.Rec)) {
                ButQuitter.Col = GREEN;
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) 
                    CloseWindow();
            }
            break;
        
        case OnePlayer:
            if (true) {
                
            }
            break;
        
        case TwoPlayer:
            if (true) {
                
            }
            break;
        
        case IATest:
            if (true) {
                
            }
            break;
        
        case Options:
            if (true) {
                
            }
            break;
        
        case Credits:
            if (true) {
                
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
                DrawText(Title, screenWidth/2 - MeasureText(Title, 150)/2, 20, 150, WHITE);
                DrawRectangleRec(But1vs1.Rec, But1vs1.Col);
                DrawText("1vs1", screenWidth/2 - MeasureText("1vs1", 100)/2, (screenHeight/100)*30, 100, WHITE);
                DrawRectangleRec(But1vsIA.Rec, But1vsIA.Col);
                DrawText("1vsIA", screenWidth/2 - MeasureText("1vsIA", 100)/2, (screenHeight/100)*40, 100, WHITE);
                DrawRectangleRec(ButTestIA.Rec, ButTestIA.Col);
                DrawText("Test IA", screenWidth/2 - MeasureText("Test IA", 100)/2, (screenHeight/100)*50, 100, WHITE);
                DrawRectangleRec(ButOptions.Rec, ButOptions.Col);
                DrawText("Options", screenWidth/2 - MeasureText("Options", 100)/2, (screenHeight/100)*60, 100, WHITE);
                DrawRectangleRec(ButCredits.Rec, ButCredits.Col);
                DrawText("Crédits", screenWidth/2 - MeasureText("Crédits", 100)/2, (screenHeight/100)*70, 100, WHITE);
                DrawRectangleRec(ButQuitter.Rec, ButQuitter.Col);
                DrawText("Quitter", screenWidth/2 - MeasureText("Quitter", 100)/2, (screenHeight/100)*80, 100, WHITE);
                break;
            
            case OnePlayer:
                break;
            
            case TwoPlayer:

                break; 

            case IATest:
                break;

            case Options:
                break;

            case Credits: 
                break;

            default:
                break;
            }
            

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //-----------
    return 0;
}