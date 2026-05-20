#include "VectorBoard.h"

Piece VectorBoard::getPiece(int position)
{
	return pieces.at(position);
}

bool VectorBoard::hasPiece(int position)
{
	return pieces.find(position) != pieces.end();
}

void VectorBoard::addPiece(Piece piece)
{
	if (hasPiece(piece.piecePosition)) return;

	pieces[piece.piecePosition] = piece;
}

void VectorBoard::standardBoard()
{
	for (int i = 8; i < 16; i++)
	{
		addPiece(Piece{ i, PieceType::PAWN, PieceAlliance::DARK });
	}

	for (int i = 48; i < 56; i++)
	{
		addPiece(Piece{ i, PieceType::PAWN, PieceAlliance::LIGHT });
	}

	addPiece(Piece{ 0, PieceType::ROOK, PieceAlliance::DARK });
	addPiece(Piece{ 1, PieceType::KNIGHT, PieceAlliance::DARK });
	addPiece(Piece{ 2, PieceType::BISHOP, PieceAlliance::DARK });
	addPiece(Piece{ 3, PieceType::QUEEN, PieceAlliance::DARK });
	addPiece(Piece{ 4, PieceType::KING, PieceAlliance::DARK });
	addPiece(Piece{ 5, PieceType::BISHOP, PieceAlliance::DARK });
	addPiece(Piece{ 6, PieceType::KNIGHT, PieceAlliance::DARK });
	addPiece(Piece{ 7, PieceType::ROOK, PieceAlliance::DARK });

	addPiece(Piece{ 56, PieceType::ROOK, PieceAlliance::LIGHT });
	addPiece(Piece{ 57, PieceType::KNIGHT, PieceAlliance::LIGHT });
	addPiece(Piece{ 58, PieceType::BISHOP, PieceAlliance::LIGHT });
	addPiece(Piece{ 59, PieceType::QUEEN, PieceAlliance::LIGHT });
	addPiece(Piece{ 60, PieceType::KING, PieceAlliance::LIGHT });
	addPiece(Piece{ 61, PieceType::BISHOP, PieceAlliance::LIGHT });
	addPiece(Piece{ 62, PieceType::KNIGHT, PieceAlliance::LIGHT });
	addPiece(Piece{ 63, PieceType::ROOK, PieceAlliance::LIGHT });
}
