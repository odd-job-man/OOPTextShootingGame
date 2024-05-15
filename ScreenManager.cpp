#include <stdio.h>
#include <vcruntime_string.h>
#include <windows.h>
#include "BaseObject.h"
#include "ScreenManager.h"

HANDLE hConsole;

void cs_Initial(void)
{
	SetCurrentDirectory(L"TextResource");
	CONSOLE_CURSOR_INFO stConsoleCursor;
	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;			
	HWND console = GetConsoleWindow();
	ShowWindow(console, SW_MAXIMIZE);
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hConsole, &stConsoleCursor);
}

CScreenManager* CScreenManager::_pScreenManager = nullptr;
CScreenManager* CScreenManager::GetInstance(void)
{
	if (_pScreenManager == nullptr)
	{
		_pScreenManager = new CScreenManager;
		atexit(Destroy);
	}
	return _pScreenManager;
}

void CScreenManager::synchronize_buffer(void)
{
	memcpy_s(_targetBuffer, SCREENSIZE, _tempBuffer, SCREENSIZE);
}

void CScreenManager::Buffer_Clear(void)
{
	for (int iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		memset(_targetBuffer[iCnt], ' ', dfSCREEN_WIDTH);
		_targetBuffer[iCnt][dfSCREEN_WIDTH - 1] = (char)NULL;
	}
}

void CScreenManager::Buffer_Flip(void)
{
	for (int iCnt = 0; iCnt < dfSCREEN_HEIGHT; iCnt++)
	{
		cs_MoveCursor(0, iCnt);
		printf(_targetBuffer[iCnt]);	
	}
}


void CScreenManager::Destroy(void)
{
	delete _pScreenManager;
	_pScreenManager = nullptr;
}

void CScreenManager::Sprite_Draw(const st_POS pos, char chSprite)
{
	if ( pos._x < 0 || pos._y < 0 || pos._x >= dfSCREEN_WIDTH - 1 ||  pos._y >= dfSCREEN_HEIGHT)
		return;

	_targetBuffer[pos._y][pos._x] = chSprite;
}

CScreenManager::SCREENPOINTER CScreenManager::getScreen(void)
{
	return _tempBuffer;
}

void CScreenManager::cs_MoveCursor(int iPosX, int iPosY)
{
	COORD stCoord;
	stCoord.X = iPosX;
	stCoord.Y = iPosY;
	SetConsoleCursorPosition(hConsole, stCoord);
}

void CScreenManager::cs_ClearScreen(void)
{
	for (int iCountY = 0 ; iCountY < dfSCREEN_HEIGHT; iCountY++ )
	{
		for (int iCountX = 0; iCountX < dfSCREEN_WIDTH; iCountX++ )
		{
			cs_MoveCursor(iCountX, iCountY);
			printf(" ");
		}
	}
}
