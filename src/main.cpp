#include "raylib.h"
#include "gui.cpp"
#include "board.h"
#include "VectorBoard.h"

int main()
{
    InitWindow(800, 800, "InkChess");
    SetTargetFPS(30);

    Board* board = new VectorBoard;
    board->addPiece(Piece{ 60, PieceType::KING, PieceAlliance::LIGHT });
    board->addPiece(Piece{ 56, PieceType::ROOK, PieceAlliance::LIGHT });
    board->addPiece(Piece{ 63, PieceType::ROOK, PieceAlliance::LIGHT });


    board->addPiece(Piece{ 39, PieceType::KNIGHT, PieceAlliance::DARK });

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