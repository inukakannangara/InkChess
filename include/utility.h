#pragma once
#include <set>

inline bool isPositionInBounds(int position)
{
	return (position >= 0) && (position <= 63);
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

inline bool isPositionThreatened(int threatPosition, PieceAlliance threatAlliance, const set<Move>& moves)
{
	Move threatMove;
	threatMove.destinationPosition = threatPosition;
	threatMove.startingPosition = INT_MIN;

	auto iter = moves.lower_bound(threatMove);

	if (iter == moves.end()) return false;

	if (iter->isCaptureMove)
	{
		if (iter->destinationPosition == threatPosition && iter->movingPiece.pieceAlliance == threatAlliance)
		{
			return true;
		}
	}
	return false;
}