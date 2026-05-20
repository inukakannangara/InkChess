#pragma once
#include "Board.h"
#include <unordered_map>

class VectorBoard : public Board
{
private:
	std::unordered_map<int, Piece> pieces;

public:
	Piece getPiece(int position) override;
	bool hasPiece(int position) override;
	void addPiece(Piece piece) override;
};