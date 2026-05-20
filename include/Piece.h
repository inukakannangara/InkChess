#pragma once
#include <functional>

enum PieceType
{
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING
};

enum PieceAlliance
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