#include "SceneOver.h"
#include "ScreenManager.h"
#include "SceneManager.h"
#include "FileManager.h"

void CSceneOver::Update()
{
	CSceneManager::GetInstance()->going_to_terminate();
	CScreenManager::GetInstance()->synchronize_buffer();
	CScreenManager::GetInstance()->Buffer_Flip();
	CScreenManager::GetInstance()->Buffer_Clear();
}

CSceneOver::CSceneOver()
{
	CFileManager::GetInstance()->LoadScreenFile("BYEBYE.txt");
}


