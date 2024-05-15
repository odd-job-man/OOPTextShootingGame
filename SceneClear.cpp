#include "windows.h"
#include "SceneClear.h"
#include "SceneManager.h"
#include "FileManager.h"
#include "ScreenManager.h"

CSceneClear::CSceneClear()
{
	CFileManager::GetInstance()->LoadScreenFile("CLEAR.txt");
}

void CSceneClear::Update()
{
	CScreenManager::GetInstance()->synchronize_buffer();
	CScreenManager::GetInstance()->Buffer_Flip();
	CScreenManager::GetInstance()->Buffer_Clear();
	if(GetAsyncKeyState(VK_TAB) & 0x8001)
		CSceneManager::GetInstance()->requestScene(GAME);
}
