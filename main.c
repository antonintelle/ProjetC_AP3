<<<<<<< Updated upstream
#include <stdio.h>
#include <stdbool.h>

int main(){
    
=======
#include "C:/raylib/raylib/src/raylib.h"
#include "C:/raylib/raylib/src/raymath.h"

typedef enum GameScreen { TitleScreen = 0, OnePlayer, TwoPlayer, IATest, Options, Credits, Stop } GameScreen;

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "The GAME");

    GameScreen CurrentScreen = TitleScreen; 

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        switch (CurrentScreen)
        {
        case TitleScreen:
            if (true) {
                DrawRectangle((screenWidth/100)*10, (screenHeight/100)*30, (screenWidth/100)*80, (screenHeight/100)*60, BLACK);
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
                /* code */
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
>>>>>>> Stashed changes
}