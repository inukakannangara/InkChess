#include "raylib.h"
#include "board.h"
#include <string>
#include <iostream>
#include <unordered_set>

using namespace std;

namespace GUI
{
	static int windowSize;
	static int tileSize;

	static bool isTileSelected = false;
	static int selectedTilePosition;

	static set<Move> legalMoves;

	static bool hasTexturesLoaded = false;

	static const string basePath = "res/piece-images/";
	static string getPiecePath(Piece piece)
	{
		std::string piecePath = basePath;

		switch (piece.pieceAlliance)
		{
			case PieceAlliance::DARK:
			{
				piecePath += "black";
				break;
			}
			case PieceAlliance::LIGHT:
			{
				piecePath += "white";
				break;
			}
		}

		piecePath += "-";

		switch (piece.pieceType)
			{
			case PieceType::PAWN:
			{
				piecePath += "pawn";
				break;
			}
			case PieceType::KNIGHT:
			{
				piecePath += "knight";
				break;
			}
			case PieceType::BISHOP:
			{
				piecePath += "bishop";
				break;
			}
			case PieceType::ROOK:
			{
				piecePath += "rook";
				break;
			}
			case PieceType::QUEEN:
			{
				piecePath += "queen";
				break;
			}
			case PieceType::KING:
			{
				piecePath += "king";
				break;
			}
		}

		piecePath += ".png";
		return piecePath;
	}

	static unordered_map<string, Texture2D> pieceTextureMap;
	static int textureSize;

	static void loadTextures()
	{
		Texture2D texture;
		for (int pieceAlliance = 0; pieceAlliance < 2; pieceAlliance++)
		{
			for (int pieceType = 0; pieceType < 6; pieceType++)
			{
				Piece piece{ 0, static_cast<PieceType>(pieceType), static_cast<PieceAlliance>(pieceAlliance) };
				string piecePath = getPiecePath(piece);
				texture = LoadTexture(piecePath.c_str());

				pieceTextureMap[piecePath] = texture;

				if (pieceAlliance == 0 && pieceType == 0)
				{
					textureSize = texture.width;
				}
			}
		}

		hasTexturesLoaded = true;
	}

	static void drawBoard(Board& board)
	{
		if (!hasTexturesLoaded)
		{
			loadTextures();
		}

		windowSize = GetScreenWidth();

		tileSize = windowSize / 8;
		int tilePosition;

		float textureScale = tileSize / (float)textureSize;

		unordered_set<int> highlightTiles;
		if (board.hasPiece(selectedTilePosition) && isTileSelected)
		{
			legalMoves = board.getLegalMoves();
			for (auto& move : legalMoves)
			{
				if (move.movingPiece == board.getPiece(selectedTilePosition))
				{
					highlightTiles.insert(move.destinationPosition);
				}
			}
		}
		
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				tilePosition = j * 8 + i;

				bool isDark = (i + j) % 2;
				Color color = isDark ? DARKGRAY : BEIGE;

				if (isTileSelected && tilePosition == selectedTilePosition)
				{
					color = PURPLE;
				}

				if (highlightTiles.count(tilePosition))
				{
					color = PINK;
				}

				DrawRectangle(
					i * tileSize,
					j * tileSize,
					tileSize,
					tileSize,
					color
				);

				if (board.hasPiece(tilePosition))
				{
					string piecePath = getPiecePath(board.getPiece(tilePosition));
					DrawTextureEx(pieceTextureMap.at(piecePath), { (float)i * tileSize, (float)j * tileSize }, 0.0f, textureScale, WHITE);
				}

				DrawText(TextFormat("%d", tilePosition), i * tileSize + tileSize*0.1, j * tileSize + tileSize*0.1, 10, BLACK);
			}
		}
	}
	
	static void handleInput(Board& board)
	{
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
		{
			Vector2 mousePos = GetMousePosition();

			int i = mousePos.x / tileSize;
			int j = mousePos.y / tileSize;

			int currentSelectedTilePosition = j * 8 + i;

			if (isTileSelected && currentSelectedTilePosition == selectedTilePosition)
			{
				isTileSelected = false;
				return;
			}

			if (!isTileSelected)
			{
				isTileSelected = true;
				selectedTilePosition = currentSelectedTilePosition;
				return;
			}
			
			Move attemptMove{ selectedTilePosition, currentSelectedTilePosition, Piece{}, false, Piece{} };
			auto moveIter = legalMoves.find(attemptMove);
			if (moveIter != legalMoves.end())
			{
				board.makeMove(*moveIter);
				isTileSelected = false;
				legalMoves = {};
				return;
			}
		}

		if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
		{
			isTileSelected = false;
		}
	}
}