#include <corecrt_memcpy_s.h>
#include "SceneBase.h"
#include "SceneGame.h"
#include "SceneManager.h"
#include "FileManager.h"
#include "ObjectManager.h"
#include "ScreenManager.h"
#include "FrameManager.h"

CSceneGame::CSceneGame(const char* pszFileName)
{
	CFileManager::GetInstance()->LoadScreenFile(pszFileName);
	
	CScreenManager::SCREENPOINTER Buffer = CScreenManager::GetInstance()->getScreen();
	// 가져온 파일버퍼의 내용을 적당히 편집한다.
	// 동시에 적 객체 초기화
	for (int i = 0; i < dfSCREEN_HEIGHT; ++i)
	{
		for (int j = 0; j < dfSCREEN_WIDTH; ++j)
		{
			if (Buffer[i][j] == '*' || Buffer[i][j] == ' ' || Buffer[i][j] == '\0')
				continue;
			
			if (Buffer[i][j] != '(')
				CObjectManager::GetInstance()->createEnemy(Buffer[i][j], st_POS{ j,i });
			Buffer[i][j] = ' ';
		}
	} 
	
	CObjectManager::GetInstance()->createPlayer();
	CFrameManager::GetInstance()->set();

}

CSceneGame::~CSceneGame()
{
	CObjectManager::GetInstance()->deleteAllobject();
}

void CSceneGame::Update(void) 
{
	CObjectManager::GetInstance()->update();
	Scene state = CObjectManager::GetInstance()->isStageClear();
	if(state != NOCHANGE)
		CSceneManager::GetInstance()->requestScene(state);

	CScreenManager::GetInstance()->synchronize_buffer();
	if (CFrameManager::GetInstance()->isSleep())
		CFrameManager::GetInstance()->sleep();
	else
		return;

	CObjectManager::GetInstance()->render();
	CScreenManager::GetInstance()->Buffer_Flip();
	CScreenManager::GetInstance()->Buffer_Clear();


	CFrameManager::GetInstance()->updateFrame();
}
