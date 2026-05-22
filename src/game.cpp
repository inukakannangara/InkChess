#include "game.h"
#include "raylib.h"
#include "VectorBoard.h"
#include "gui.h"
#include <memory>

using namespace std;

unique_ptr<Board> board;

void Game::initialize()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(1000, 800, "InkChess");
    SetTargetFPS(60);

    board = make_unique<VectorBoard>();
    board->standardBoard();
}
void Game::startHumanGame()
{
    initialize();
    humanGameLoop();
}

set<Move> selectedPieceLegalMoves;

void Game::humanGameLoop()
{
    while (!WindowShouldClose())
    {
        BeginDrawing();
        GUI::drawBoard(*board, 0, 0, GetScreenHeight());
        GUI::handleInput();
        EndDrawing();

        int startingPosition;
        int destinationPosition;

        if (GUI::isStartingPositionSelected(startingPosition)
            && board->hasPiece(startingPosition, board->getPlayingAlliance())
            && selectedPieceLegalMoves.empty())
        {
            set<Move>& legalMoves = board->getLegalMoves();

            for (const Move& move : legalMoves)
            {
                if (move.startingPosition == startingPosition)
                {
                    GUI::addHighlightTile(move.destinationPosition, PINK);
                    selectedPieceLegalMoves.insert(move);
                }
            }
        }

        if (GUI::isDestinationPositionSelected(destinationPosition))
        {
            Move move;
            move.startingPosition = startingPosition;
            move.destinationPosition = destinationPosition;

            auto iter = selectedPieceLegalMoves.lower_bound(move);

            if (iter != selectedPieceLegalMoves.end())
            {
                if (iter->startingPosition == startingPosition && iter->destinationPosition == destinationPosition)
                {
                    board->makeMove(*iter, true);
                }
            }

            GUI::resetDestinationPosition();
            GUI::resetStartingPosition();
            GUI::resetHighlightTiles();
            selectedPieceLegalMoves.clear();
        }
    }

    CloseWindow();
}