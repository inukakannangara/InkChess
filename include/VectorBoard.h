#pragma once
#include "Board.h"
#include "Piece.h"
#include <unordered_map>
#include <unordered_set>

using namespace std;

class VectorBoard : public Board
{
private:
	unordered_map<int, Piece> lightPieces;
	unordered_map<int, Piece> darkPieces;

	inline bool isPositionInBounds(int position)
	{
		return position >= 0 && position <= 63;
	}

	inline int getRank(int position)
	{
		return 7 - position / 8;
	}

	inline bool isInRank(int position, int rank)
	{
		int actualRank = 7 - position / 8;
		return actualRank == rank;
	}

	inline bool isInFile(int position, int file)
	{
		return (position - file) % 8 == 0;
	}

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

	inline unordered_set<int> getDestinationPositions(Piece piece)
	{
		switch (piece.pieceType)
		{
		case::PieceType::PAWN:
		{
			return getPawnDestinationPositions(piece.piecePosition, piece.pieceAlliance);
			break;
		}
		case::PieceType::KNIGHT:
		{
			return getKnightDestinationPositions(piece.piecePosition, piece.pieceAlliance);
			break;
		}
		case::PieceType::BISHOP:
		{
			return getBishopDestinationPositions(piece.piecePosition, piece.pieceAlliance);
			break;
		}
		case::PieceType::ROOK:
		{
			return getRookDestinationPositions(piece.piecePosition, piece.pieceAlliance);
			break;
		}
		case::PieceType::QUEEN:
		{
			return getQueenDestinationPositions(piece.piecePosition, piece.pieceAlliance);
			break;
		}
		case::PieceType::KING:
		{
			return getKingDestinationPositions(piece.piecePosition, piece.pieceAlliance);
			break;
		}
		}
	}
	inline unordered_set<int> getPawnDestinationPositions(int position, PieceAlliance alliance)
	{
		unordered_set<int> destinations;

		int direction = alliance == PieceAlliance::LIGHT ? -1 : 1;
		int destination;

		for (int i = 8; i <= 16; i += 8)
		{
			destination = position + i * direction;

			if (i == 16)
			{
				int allowedRank = (alliance == PieceAlliance::LIGHT) ? 1 : 6;
				if (!isInRank(position, allowedRank)) break;
			}

			if (hasPiece(destination)) break;
			destinations.insert(destination);
		}

		unordered_set<int> offsets;

		if (!isInFile(position, 0))
		{
			if (alliance == PieceAlliance::LIGHT)
			{
				offsets.insert(-9);
			}
			else
			{
				offsets.insert(7);
			}
		}
		if (!isInFile(position, 7))
		{
			if (alliance == PieceAlliance::LIGHT)
			{
				offsets.insert(-7);
			}
			else
			{
				offsets.insert(9);
			}
		}

		for (int offset : offsets)
		{
			destination = position + offset;
			if (hasPiece(destination))
			{
				Piece& piece = getPiece(destination);
				if (piece.pieceAlliance != alliance)
				{
					destinations.insert(destination);
				}
			}
		}

		return destinations;
	}
	inline unordered_set<int> getKnightDestinationPositions(int position, PieceAlliance alliance)
	{
		unordered_set<int> offsets = {10, 17, 15, 6, -10, -17, -15, -6};
		
		if (isInFile(position, 1)) 
		{
			offsets.erase(-10);
			offsets.erase(6);
		}

		if (isInFile(position, 6))
		{
			offsets.erase(-6);
			offsets.erase(10);
		}

		if (isInFile(position, 0))
		{
			offsets.erase(-17);
			offsets.erase(-10);
			offsets.erase(6);
			offsets.erase(15);
		}

		if (isInFile(position, 7))
		{
			offsets.erase(17);
			offsets.erase(10);
			offsets.erase(-6);
			offsets.erase(-15);
		}

		unordered_set<int> destinationPositions;

		int destination;
		for (int offset : offsets)
		{
			destination = position + offset;

			if (!isPositionInBounds(destination)) continue;
			
			switch (alliance)
			{
			case::PieceAlliance::LIGHT:
			{
				if (lightPieces.find(destination) != lightPieces.end()) continue;
				break;
			}
			case::PieceAlliance::DARK:
			{
				if (darkPieces.find(destination) != darkPieces.end()) continue;
				break;
			}
			}

			destinationPositions.insert(destination);
		}

		return destinationPositions;
	}
	inline unordered_set<int> getBishopDestinationPositions(int position, PieceAlliance alliance)
	{
		unordered_set<int> destinations;
		
		vector<int> offsets;

		if (!isInFile(position, 7))
		{
			offsets.push_back(9);
			offsets.push_back(-7);
		}
	
		if (!isInFile(position, 0))
		{
			offsets.push_back(-9);
			offsets.push_back(7);
		}

		int destination = position;
		for (int offset : offsets)
		{
			while (isPositionInBounds(destination))
			{
				destination = destination + offset;
				if (handlePieceBlock(destination, destinations, alliance)) break;

				if (isInRank(destination, 0) || isInRank(destination, 7) || isInFile(destination, 0) || isInFile(destination, 7)) break;
			}
			destination = position;
		}

		return destinations;
	}
	inline unordered_set<int> getRookDestinationPositions(int position, PieceAlliance alliance)
	{
		unordered_map<int, Piece>& movingPieces = (alliance == PieceAlliance::LIGHT) ? lightPieces : darkPieces;
		unordered_map<int, Piece>& opponentPieces = (alliance == PieceAlliance::LIGHT) ? darkPieces : lightPieces;

		unordered_set<int> destinations;
		int verticalDestination = position + 8;

		while (isPositionInBounds(verticalDestination))
		{
			if (handlePieceBlock(verticalDestination, destinations, alliance)) break;
			verticalDestination = verticalDestination + 8;
		}

		verticalDestination = position - 8;

		while (isPositionInBounds(verticalDestination))
		{
			if (handlePieceBlock(verticalDestination, destinations, alliance)) break;
			verticalDestination = verticalDestination - 8;
		}

		int horizontalDestination = position + 1;

		while (isInRank(horizontalDestination, getRank(position)))
		{
			if (handlePieceBlock(horizontalDestination, destinations, alliance)) break;
			horizontalDestination = horizontalDestination + 1;
		}

		horizontalDestination = position - 1;

		while (isInRank(horizontalDestination, getRank(position)))
		{
			if (handlePieceBlock(horizontalDestination, destinations, alliance)) break;
			horizontalDestination = horizontalDestination - 1;
		}

		return destinations;
	}
	inline unordered_set<int> getQueenDestinationPositions(int position, PieceAlliance alliance)
	{
		unordered_set<int> bishopDestinations = getBishopDestinationPositions(position, alliance);
		unordered_set<int>& rookDestinations = getRookDestinationPositions(position, alliance);

		bishopDestinations.insert(rookDestinations.begin(), rookDestinations.end());

		return bishopDestinations;
	}
	inline unordered_set<int> getKingDestinationPositions(int position, PieceAlliance alliance)
	{
		unordered_set<int> destinations;

		vector<int> offsets{ 8, -8 };

		if (!isInFile(position, 0))
		{
			offsets.push_back(-1);
			offsets.push_back(7);
			offsets.push_back(-9);
		}

		if (!isInFile(position, 7))
		{
			offsets.push_back(1);
			offsets.push_back(-7);
			offsets.push_back(9);
		}

		int destination;
		for (int offset : offsets)
		{
			destination = position + offset;

			if (hasPiece(destination))
			{
				Piece& piece = getPiece(destination);
				if (piece.pieceAlliance != alliance)
				{
					destinations.insert(destination);
				}
			}
			else
			{
				destinations.insert(destination);
			}
		}

		return destinations;
	}

public:
	Piece getPiece(int position) override;
	bool hasPiece(int position) override;
	void addPiece(const Piece& piece) override;
	void removePiece(const Piece& piece) override;

	set<Move> getPseudoLegalMoves();
	set<Move> getLegalMoves() override;

	void makeMove(const Move& move, bool changeTurn) override;

	int getKingPosition(PieceAlliance alliance) override;

	void standardBoard() override;
};