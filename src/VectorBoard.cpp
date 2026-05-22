#include "VectorBoard.h"

using namespace std;

Piece VectorBoard::getPiece(int position)
{
	if (lightPieces.find(position) != lightPieces.end())
	{
		return lightPieces.at(position);
	}
	else
	{
		return darkPieces.at(position);
	}
}

PieceAlliance VectorBoard::getPlayingAlliance()
{
	return playingAlliance;
}

bool VectorBoard::hasPiece(int position)
{
	return lightPieces.count(position) || darkPieces.count(position);
}

bool VectorBoard::hasPiece(int position, PieceAlliance alliance)
{
	unordered_map<int, Piece>& pieces = (alliance == PieceAlliance::LIGHT) ? lightPieces : darkPieces;
	return pieces.count(position);
}

void VectorBoard::addPiece(const Piece& piece)
{
	if (hasPiece(piece.piecePosition)) return;

	if (piece.pieceAlliance == PieceAlliance::LIGHT)
	{
		lightPieces[piece.piecePosition] = piece;
		return;
	}

	if (piece.pieceAlliance == PieceAlliance::DARK)
	{
		darkPieces[piece.piecePosition] = piece;
		return;
	}
}

void VectorBoard::removePiece(const Piece& piece)
{
	unordered_map<int, Piece>& pieces = piece.pieceAlliance == PieceAlliance::LIGHT ? lightPieces : darkPieces;
	pieces.erase(piece.piecePosition);
}

set<Move> VectorBoard::getPseudoLegalMoves(PieceAlliance alliance)
{
	set<Move> pseudoLegalMoves;

	unordered_map<int, Piece>& playingPieces = (alliance == PieceAlliance::LIGHT) ? lightPieces : darkPieces;
	unordered_map<int, Piece>& opponentPieces = (alliance == PieceAlliance::LIGHT)? darkPieces : lightPieces;

	for (auto& p : playingPieces)
	{
		Piece piece = p.second;
		unordered_set<int> pieceDestinationPositions = getDestinationPositions(piece);

		for (int destination : pieceDestinationPositions)
		{
			Move move{ piece.piecePosition, destination, piece, false, piece };

			auto capturePieceIter = opponentPieces.find(destination);
			if (capturePieceIter != opponentPieces.end())
			{
				move.isCaptureMove = true;
				move.capturePiece = capturePieceIter->second;
			}

			pseudoLegalMoves.insert(move);
		}
	}
	return pseudoLegalMoves;
}

set<Move> VectorBoard::getCastleMoves()
{
	set<Move> castleMoves;

	int requiredKingPosition = (playingAlliance == PieceAlliance::LIGHT) ? 60 : 4;

	if (!hasPiece(requiredKingPosition)) return castleMoves;
	Piece& king = getPiece(requiredKingPosition);
	if (king.pieceType != PieceType::KING || king.numTimesMoved > 0 || king.pieceAlliance != playingAlliance) return castleMoves;

	int requiredRookPosition = (playingAlliance == PieceAlliance::LIGHT) ? 63 : 7;

	for (int queenSideCastle = 0; queenSideCastle <= 1; queenSideCastle++)
	{
		requiredRookPosition = requiredRookPosition - queenSideCastle * 7;

		if (!hasPiece(requiredRookPosition)) continue;
		Piece& rook = getPiece(requiredRookPosition);
		if (rook.pieceType != PieceType::ROOK || rook.numTimesMoved > 0 || rook.pieceAlliance != playingAlliance) continue;

		Move castleMove;
		bool moveCreated = createCastleMove(king, rook, queenSideCastle, castleMove);
		if (moveCreated)
		{
			castleMoves.insert(castleMove);
		}
	}

	return castleMoves;
}

set<Move> VectorBoard::getLegalMoves()
{
	PieceAlliance opponentAlliance = (playingAlliance == PieceAlliance::LIGHT) ? PieceAlliance::DARK : PieceAlliance::LIGHT;

	set<Move> pseudoLegalMoves = getPseudoLegalMoves(playingAlliance);
	set<Move> legalMoves;

	int prevKingPosition = getKingPosition(playingAlliance);
	int newKingPosition;

	for (const Move& pseudoMove : pseudoLegalMoves)
	{
		VectorBoard board = *this;
		board.makeMove(pseudoMove, true);

		if (pseudoMove.movingPiece.pieceType == PieceType::KING)
		{
			newKingPosition = pseudoMove.destinationPosition;
		}
		else
		{
			newKingPosition = prevKingPosition;
		}

		set<Move> opponentLegalMoves = board.getPseudoLegalMoves(opponentAlliance);

		Move kingAttackingMove;
		kingAttackingMove.destinationPosition = newKingPosition;
		kingAttackingMove.startingPosition = INT_MIN;

		auto iter = opponentLegalMoves.lower_bound(kingAttackingMove);

		if (iter == opponentLegalMoves.end())
		{
			legalMoves.insert(pseudoMove);
			continue;
		}

		if (iter->destinationPosition != newKingPosition)
		{
			legalMoves.insert(pseudoMove);
		}
	}

	set<Move> castleMoves = getCastleMoves();
	legalMoves.merge(castleMoves);

	return legalMoves;
}

void VectorBoard::makeMove(const Move& move, bool changeTurn)
{
	removePiece(move.movingPiece);
	
	Piece movedPiece = move.movingPiece;
	movedPiece.piecePosition = move.destinationPosition;
	movedPiece.numTimesMoved++;

	if (move.isCaptureMove)
	{
		removePiece(move.capturePiece);
	}

	addPiece(movedPiece);

	if (move.isKingSideCastleMove)
	{
		Piece rook = move.movingRook;
		removePiece(move.movingRook);

		rook.numTimesMoved++;
		rook.piecePosition = (move.movingPiece.pieceAlliance == PieceAlliance::LIGHT) ? 61 : 5;
		addPiece(rook);
	}

	if (move.isQueenSideCastleMove)
	{
		Piece rook = move.movingRook;
		removePiece(move.movingRook);

		rook.numTimesMoved++;
		rook.piecePosition = (move.movingPiece.pieceAlliance == PieceAlliance::LIGHT) ? 59 : 3;
		addPiece(rook);
	}

	if (changeTurn)
	{
		playingAlliance = (move.movingPiece.pieceAlliance == PieceAlliance::LIGHT) ? PieceAlliance::DARK : PieceAlliance::LIGHT;
	}
}

int VectorBoard::getKingPosition(PieceAlliance alliance)
{
	unordered_map<int, Piece>& pieces = (alliance == PieceAlliance::LIGHT) ? lightPieces : darkPieces;

	for (auto& p : pieces)
	{
		if (p.second.pieceType == PieceType::KING)
		{
			return p.second.piecePosition;
		}
	}
	return -1;
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

	playingAlliance = PieceAlliance::LIGHT;
}

bool VectorBoard::createCastleMove(const Piece& king, const Piece& rook, bool isQueenSideCastle, Move& castleMove)
{
	vector<int> castleSpaces;

	int kingDestinationPosition;

	if (isQueenSideCastle)
	{
		if (playingAlliance == PieceAlliance::LIGHT)
		{
			castleSpaces.push_back(57);
			castleSpaces.push_back(58);
			castleSpaces.push_back(59);

			kingDestinationPosition = 58;
		}
		else
		{
			castleSpaces.push_back(1);
			castleSpaces.push_back(2);
			castleSpaces.push_back(3);

			kingDestinationPosition = 2;
		}
	}
	else
	{
		if (playingAlliance == PieceAlliance::LIGHT)
		{
			castleSpaces.push_back(61);
			castleSpaces.push_back(62);

			kingDestinationPosition = 62;
		}
		else
		{
			castleSpaces.push_back(5);
			castleSpaces.push_back(6);

			kingDestinationPosition = 6;
		}
	}

	for (int castleSpace : castleSpaces)
	{
		if (hasPiece(castleSpace))
		{
			return false;
		}
	}

	PieceAlliance opponentAlliance = (playingAlliance == PieceAlliance::LIGHT) ? PieceAlliance::DARK : PieceAlliance::LIGHT;
	set<Move> opponentLegalMoves = getPseudoLegalMoves(opponentAlliance);

	if (isPositionThreatened(king.piecePosition, opponentAlliance, opponentLegalMoves)) return false;

	for (int castleSpace : castleSpaces) 
	{
		if (isPositionThreatened(castleSpace, opponentAlliance, opponentLegalMoves))
		{
			return false;
		}
	}

	castleMove.startingPosition = king.piecePosition;
	castleMove.movingPiece = king;
	castleMove.destinationPosition = kingDestinationPosition;
	
	if (isQueenSideCastle)
	{
		castleMove.isQueenSideCastleMove = true;
		castleMove.isKingSideCastleMove = false;
	}
	else
	{
		castleMove.isKingSideCastleMove = true;
		castleMove.isQueenSideCastleMove = false;
	}
	castleMove.movingRook = rook;

	return true;
}

unordered_set<int> VectorBoard::getDestinationPositions(Piece piece)
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

unordered_set<int> VectorBoard::getPawnDestinationPositions(int position, PieceAlliance alliance)
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

unordered_set<int> VectorBoard::getKnightDestinationPositions(int position, PieceAlliance alliance)
{
	unordered_set<int> offsets = { 10, 17, 15, 6, -10, -17, -15, -6 };

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

unordered_set<int> VectorBoard::getBishopDestinationPositions(int position, PieceAlliance alliance)
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

	int destination;
	for (int offset : offsets)
	{
		destination = position + offset;
		while (isPositionInBounds(destination))
		{
			if (handlePieceBlock(destination, destinations, alliance)) break;

			if (isInRank(destination, 0) || isInRank(destination, 7) || isInFile(destination, 0) || isInFile(destination, 7)) break;

			destination = destination + offset;
		}
		destination = position;
	}

	return destinations;
}

unordered_set<int> VectorBoard::getRookDestinationPositions(int position, PieceAlliance alliance)
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

	while (isPositionInBounds(horizontalDestination) && isInRank(horizontalDestination, getRank(position)))
	{
		if (handlePieceBlock(horizontalDestination, destinations, alliance)) break;
		horizontalDestination = horizontalDestination + 1;
	}

	horizontalDestination = position - 1;

	while (isPositionInBounds(horizontalDestination) && isInRank(horizontalDestination, getRank(position)))
	{
		if (handlePieceBlock(horizontalDestination, destinations, alliance)) break;
		horizontalDestination = horizontalDestination - 1;
	}

	return destinations;
}

unordered_set<int> VectorBoard::getQueenDestinationPositions(int position, PieceAlliance alliance)
{
	unordered_set<int> bishopDestinations = getBishopDestinationPositions(position, alliance);
	unordered_set<int>& rookDestinations = getRookDestinationPositions(position, alliance);

	bishopDestinations.insert(rookDestinations.begin(), rookDestinations.end());

	return bishopDestinations;
}

unordered_set<int> VectorBoard::getKingDestinationPositions(int position, PieceAlliance alliance)
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

		if (!isPositionInBounds(destination)) continue;

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