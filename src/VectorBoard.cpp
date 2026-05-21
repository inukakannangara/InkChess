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
bool VectorBoard::hasPiece(int position)
{
	return lightPieces.find(position) != lightPieces.end() || darkPieces.find(position) != darkPieces.end();
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

set<Move> VectorBoard::getPseudoLegalMoves()
{
	set<Move> pseudoLegalMoves;

	unordered_map<int, Piece>& playingPieces = isLightTurn ? lightPieces : darkPieces;
	unordered_map<int, Piece>& opponentPieces = isLightTurn ? darkPieces : lightPieces;

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

set<Move> VectorBoard::getLegalMoves()
{
	return getPseudoLegalMoves();
}

void VectorBoard::makeMove(const Move& move)
{
	removePiece(move.movingPiece);
	
	Piece movedPiece = move.movingPiece;
	movedPiece.piecePosition = move.destinationPosition;

	if (move.isCaptureMove)
	{
		removePiece(move.capturePiece);
	}

	addPiece(movedPiece);

	isLightTurn = !(move.movingPiece.pieceAlliance == PieceAlliance::LIGHT);
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

	isLightTurn = true;
}
