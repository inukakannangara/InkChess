#include "raylib.h"
#include <unordered_set>
#include "gui.h"

using namespace std;

int tileSize;

const int nullPosition = -1;
int selectedStartingPosition = nullPosition;
int selectedDestinationPosition = nullPosition;

bool hasTexturesLoaded = false;

const string basePath = "res/piece-images/";
inline static string getPiecePath(Piece piece)
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

unordered_map<string, Texture2D> pieceTextureMap;
int textureSize;

unordered_map<int, Color> highlightTiles;

void GUI::loadTextures()
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

void GUI::drawBoard(Board& board, int startX, int startY, int size)
{
	if (!hasTexturesLoaded)
	{
		loadTextures();
	}

	ClearBackground(BLACK);

	tileSize = size / 8;

	int tilePosition;
	float textureScale = tileSize / (float)textureSize;
		
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			tilePosition = j * 8 + i;

			bool isDark = (i + j) % 2;
			Color color = isDark ? DARKGRAY : BEIGE;

			if (selectedStartingPosition == tilePosition)
			{
				color = PURPLE;
			}

			if (highlightTiles.count(tilePosition))
			{
				color = highlightTiles.at(tilePosition);
			}

			DrawRectangle(
				i * tileSize + startX,
				j * tileSize + startY,
				tileSize,
				tileSize,
				color
			);

			if (board.hasPiece(tilePosition))
			{
				string piecePath = getPiecePath(board.getPiece(tilePosition));
				DrawTextureEx(pieceTextureMap.at(piecePath), { (float)i * tileSize, (float)j * tileSize }, 0.0f, textureScale, WHITE);
			}

			//DrawText(TextFormat("%d", tilePosition), i * tileSize + tileSize*0.1, j * tileSize + tileSize*0.1, 20, BLACK);
		}
	}
}
	
void GUI::handleInput()
{
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		Vector2 mousePos = GetMousePosition();

		int i = mousePos.x / tileSize;
		int j = mousePos.y / tileSize;

		int currentTilePosition = j * 8 + i;

		if (selectedStartingPosition == nullPosition)
		{
			selectedStartingPosition = currentTilePosition;
		}
		else
		{
			selectedDestinationPosition = currentTilePosition;
		}
	}
}

bool GUI::isStartingPositionSelected(int& startingPosition)
{
	if (selectedStartingPosition == nullPosition)
	{
		return false;
	}
	else
	{
		startingPosition = selectedStartingPosition;
		return true;
	}
}

bool GUI::isDestinationPositionSelected(int& destinationPosition)
{
	if (selectedDestinationPosition == nullPosition)
	{
		return false;
	}
	else
	{
		destinationPosition = selectedDestinationPosition;
		return true;
	}
}

void GUI::addHighlightTile(int position, Color color)
{
	highlightTiles[position] = color;
}

void GUI::resetHighlightTiles()
{
	highlightTiles.erase(highlightTiles.begin(), highlightTiles.end());
}

void GUI::resetStartingPosition()
{
	selectedStartingPosition = nullPosition;
}

void GUI::resetStartingPosition(int position)
{
	selectedStartingPosition = position;
}

void GUI::resetDestinationPosition()
{
	selectedDestinationPosition = nullPosition;
}
