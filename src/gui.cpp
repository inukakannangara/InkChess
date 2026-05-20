#include "raylib.h"
#include "board.h"

namespace GUI
{
	static void drawBoard()
	{
		int size = GetScreenWidth();

		int tileSize = size / 8;

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				bool isDark = (i + j) % 2;

				Color color = isDark ? DARKGRAY : BEIGE;

				DrawRectangle(
					i * tileSize,
					j * tileSize,
					tileSize,
					tileSize,
					color
				);
			}
		}
	}
}