#include "VectorBoard.h"

using namespace std;

VectorBoard::getPiece(int position)
{
	return pieces.at(position);
}

VectorBoard::hasPiece(int position)
{
	return pieces.find(position) != pieces.end();
}

VectorBoard::addPiece(Piece piece)
{
	if (hasPiece(piece.piecePosition)) return;

	pieces[piece.piecePosition] = piece;
}