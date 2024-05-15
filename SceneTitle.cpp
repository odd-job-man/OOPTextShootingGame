#include <Windows.h>
#include "SceneManager.h"
#include "SceneTitle.h"
#include "FileManager.h"
#include "ScreenManager.h"

CSceneTitle::CSceneTitle()
{
	CFileManager::GetInstance()->LoadScreenFile("TITLE.txt");
}

void CSceneTitle::Update()
{
	CScreenManager::GetInstance()->synchronize_buffer();
	CScreenManager::GetInstance()->Buffer_Flip();
	CScreenManager::GetInstance()->Buffer_Clear();
	for (int key = 0; key < 256; key++)
	{
		// 현재 키의 상태 가져오기
		SHORT state = GetAsyncKeyState(key);

		// 키의 상태 확인
		if (state & 0x8001) {
			CSceneManager::GetInstance()->requestScene(GAME);
		}
	}
}