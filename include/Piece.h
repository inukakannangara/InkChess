#pragma once
#include <functional>

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

	bool operator==(const Piece& other) const
	{
		return piecePosition == other.piecePosition && pieceType == other.pieceType && pieceAlliance == other.pieceAlliance;
	}
};