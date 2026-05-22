#pragma once
#include "Board.h"
#include "Piece.h"
#include <unordered_map>
#include <unordered_set>
#include "utility.h"

using namespace std;

class VectorBoard : public Board
{
private:
	unordered_map<int, Piece> lightPieces;
	unordered_map<int, Piece> darkPieces;

	inline bool handlePieceBlock(int destination, unordered_set<int>& destinations, PieceAlliance movingPieceAlliance)
	{
		if (hasPiece(destination))
		{
			Piece piece = getPiece(destination);

			if (piece.pieceAlliance != movingPieceAlliance)
			{
				destinations.insert(destination);
			}

			return true;
		}
		else
		{
			destinations.insert(destination);
			return false;
		}
	}

	bool createCastleMove(const Piece& king, const Piece& rook, bool isQueenSideCastle, Move& castleMove);

	unordered_set<int> getDestinationPositions(Piece piece);

	unordered_set<int> getPawnDestinationPositions(int position, PieceAlliance alliance);
	unordered_set<int> getKnightDestinationPositions(int position, PieceAlliance alliance);

	unordered_set<int> getBishopDestinationPositions(int position, PieceAlliance alliance);
	unordered_set<int> getRookDestinationPositions(int position, PieceAlliance alliance);
	unordered_set<int> getQueenDestinationPositions(int position, PieceAlliance alliance);

	unordered_set<int> getKingDestinationPositions(int position, PieceAlliance alliance);

public:
	Piece getPiece(int position) override;
	bool hasPiece(int position) override;
	void addPiece(const Piece& piece) override;
	void removePiece(const Piece& piece) override;

	set<Move> getPseudoLegalMoves(PieceAlliance alliance);
	set<Move> getCastleMoves();
	set<Move> getLegalMoves() override;

	void makeMove(const Move& move, bool changeTurn) override;

	int getKingPosition(PieceAlliance alliance) override;

	void standardBoard() override;
};