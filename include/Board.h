#pragma once
#include "Piece.h"

class Board
{
public:
	virtual Piece getPiece(int position) = 0;
	virtual bool hasPiece(int position) = 0;
	virtual void addPiece(Piece piece) = 0;
};