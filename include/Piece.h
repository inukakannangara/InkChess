#pragma once
#include <functional>
#include <vector>

using namespace std;

enum class PieceType
{
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING
};

enum class PieceAlliance
{
	DARK,
	LIGHT
};

struct Piece 
{
	int piecePosition;
	PieceType pieceType;
	PieceAlliance pieceAlliance;

	int numTimesMoved = 0;

	bool operator==(const Piece& other) const
	{
		return piecePosition == other.piecePosition && pieceType == other.pieceType && pieceAlliance == other.pieceAlliance;
	}
};