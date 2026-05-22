#pragma once
#include "Piece.h"

struct Move
{
	int startingPosition;
	int destinationPosition;

	Piece movingPiece;

	bool isCaptureMove = false;
	Piece capturePiece;

	bool isKingSideCastleMove = false;
	bool isQueenSideCastleMove = false;

	Piece movingRook;

	bool operator<(const Move& other) const
	{
		if (destinationPosition == other.destinationPosition)
		{
			return startingPosition < other.startingPosition;
		}

		return destinationPosition < other.destinationPosition;
	}
};