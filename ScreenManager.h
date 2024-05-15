#pragma once
#include "public_const.h"
#include "BaseObject.h"

void cs_Initial(void);

class CScreenManager
{
public:
	using SCREENPOINTER = char(*)[dfSCREEN_WIDTH];
	static CScreenManager* GetInstance(void);
	void synchronize_buffer(void);
	void Buffer_Clear(void);
	void Buffer_Flip(void);
	void Sprite_Draw(const st_POS pos, char chSprite);
	SCREENPOINTER getScreen(void);
private:
	char _targetBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];
	char _tempBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];
	static CScreenManager* _pScreenManager;
	CScreenManager() = default;
	static void Destroy(void);
	void cs_MoveCursor(int iPosX, int iPosY);
	void cs_ClearScreen(void);
};