#include <stdio.h>
#include "RenderSystem.h"

namespace render
{
	bool bScreenIndex;
	HANDLE hScreen[2];

	SMALL_RECT consoleScreenSize;
	INT consoleScreenX;
	INT consoleScreenY;

	SMALL_RECT updateScreenSize;
	INT updateScreenX;
	INT updateScreenY;

	CONSOLE_SCREEN_BUFFER_INFO csbi;

	SMALL_RECT GetPlayerMovableRect()
	{
		return updateScreenSize;
	}

	HANDLE GetScreenHandle()
	{
		int index = (bScreenIndex ? 1 : 0);

		return hScreen[index];
	}

	void GotoXY(int x, int y)
	{
		COORD Cur = { x, y };

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
	}

	void InitScreen()
	{
		hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);


		CONSOLE_CURSOR_INFO cursorInfo = { 0, };
		cursorInfo.bVisible = 0;
		cursorInfo.dwSize = 1;
		SetConsoleCursorInfo(hScreen[0], &cursorInfo);
		SetConsoleCursorInfo(hScreen[1], &cursorInfo);

		CONSOLE_SCREEN_BUFFER_INFO csbi;
		//system("mode con:cols=100 lines=10");
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

		consoleScreenSize.Bottom = csbi.srWindow.Bottom;
		consoleScreenSize.Left = csbi.srWindow.Left;
		consoleScreenSize.Right = csbi.srWindow.Right;
		consoleScreenSize.Top = csbi.srWindow.Top;

		consoleScreenX = csbi.dwSize.X;
		consoleScreenY = csbi.dwSize.Y;

		updateScreenSize.Bottom = csbi.srWindow.Bottom-2;
		updateScreenSize.Left = csbi.srWindow.Left;
		updateScreenSize.Right = csbi.srWindow.Right-2;
		updateScreenSize.Top = csbi.srWindow.Top;

		updateScreenX = consoleScreenX;
		updateScreenY = consoleScreenY;
	}

	void ScreenFlipping()
	{
		SetConsoleActiveScreenBuffer(GetScreenHandle());
		bScreenIndex = !bScreenIndex;
	}

	void ScreenClear()
	{
		COORD Coor = { updateScreenSize.Left,updateScreenSize.Top };
		DWORD dw;

		for (int y = 0; y < updateScreenY; y++)
		{
			Coor.Y = updateScreenSize.Top + y;

			FillConsoleOutputCharacter(GetScreenHandle(), ' ', updateScreenX, Coor, &dw);
		}
	}

	void ScreenRelease()
	{
		CloseHandle(hScreen[0]);
		CloseHandle(hScreen[1]);
	}

	void ScreenDraw(int x, int y, const char c)
	{
		DWORD dw;
		COORD Cur = { x,y };
		char buffer[10];
		sprintf_s(buffer, "%c", c);

		SetConsoleCursorPosition(GetScreenHandle(), Cur);

		WriteFile(GetScreenHandle(), buffer, 1, &dw, NULL);
	}

	void ScreenDraw(int x, int y, const char* pStr)
	{
		DWORD dw;
		COORD Cur = { x,y };

		SetConsoleCursorPosition(GetScreenHandle(), Cur);

		WriteFile(GetScreenHandle(), pStr, strlen(pStr), &dw, NULL);
	}

	void DrawBorder()
	{
		for (int x = updateScreenSize.Left + 1; x < updateScreenSize.Right + 1; x++)
		{
			ScreenDraw(x, updateScreenSize.Top + 1, '#');
		}
		for (int x = updateScreenSize.Left + 1; x < updateScreenSize.Right + 1; x++)
		{
			ScreenDraw(x, updateScreenSize.Bottom + 1, '#');
		}
		for (int x = updateScreenSize.Top + 1; x < updateScreenSize.Bottom + 1; x++)
		{
			ScreenDraw( updateScreenSize.Left + 1,x, '#');
		}
		for (int x = updateScreenSize.Top + 1; x < updateScreenSize.Bottom + 1; x++)
		{
			ScreenDraw( updateScreenSize.Right + 1,x, '#');
		}
	}

	void FailScene()
	{
		ScreenDraw(40, 20, R"(아... 그거 그렇게 하는거 아닌데 ㅋㅋ;
)");
		
	}
}