#pragma once
#include "Piece.h"
#include "Move.h"
#include <vector>
#include <set>

class Board
{
protected:
	PieceAlliance playingAlliance = PieceAlliance::LIGHT;

public:
	virtual Piece getPiece(int position) = 0;
	virtual bool hasPiece(int position) = 0;
	virtual void addPiece(const Piece& piece) = 0;
	virtual void removePiece(const Piece& piece) = 0;

	virtual std::set<Move> getLegalMoves() = 0;
	virtual void makeMove(const Move& move, bool changeTurn) = 0;

	virtual int getKingPosition(PieceAlliance alliance) = 0;

	virtual void standardBoard() = 0;
};