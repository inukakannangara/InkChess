#include "raylib.h"
#include "gui.cpp"

int main()
{
    InitWindow(600, 600, "InkChess");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        GUI::drawBoard();
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}