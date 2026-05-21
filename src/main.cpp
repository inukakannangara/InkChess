#include "raylib.h"
#include "gui.cpp"
#include "board.h"
#include "VectorBoard.h"

int main()
{
    InitWindow(800, 800, "InkChess");
    SetTargetFPS(30);

    Board* board = new VectorBoard;
    board->standardBoard();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        GUI::drawBoard(*board);
        GUI::handleInput(*board);
        EndDrawing();
    }
    
    delete board;

    CloseWindow();
    return 0;
}