#pragma once
#include "raylib.h"
#include "Board.h"
#include <memory>
#include <string>

namespace GUI
{
	void loadTextures();
	void drawBoard(Board& board, int startX, int startY, int size);
	void handleInput();

	bool isStartingPositionSelected(int& startingPosition);
	bool isDestinationPositionSelected(int& destinationPosition);

	void addHighlightTile(int position, Color color);
	void resetHighlightTiles();

	void resetStartingPosition();
	void resetStartingPosition(int position);
	void resetDestinationPosition();
}