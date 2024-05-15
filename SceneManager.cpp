#include <stdlib.h>
#include "SceneBase.h"
#include "SCeneManager.h"
#include "SceneGame.h"
#include "SceneTitle.h"
#include "SceneClear.h"
#include "SceneOver.h"
#include "FileManager.h"
#include <string.h>
CSceneManager* CSceneManager::_pSceneManager = nullptr;

CSceneManager* CSceneManager::GetInstance(void)
{
	if (_pSceneManager == nullptr)
	{
		_pSceneManager = new CSceneManager;
		atexit(Destroy);
	}

	return _pSceneManager;
}

void CSceneManager::run()
{
	while (true)
	{
		if (_isSceneChange)
		{
			_isSceneChange = false;
			delete _pScene;
			switch (current_scene)
			{
			case TITLE:
				_pScene = new CSceneTitle;
				break;
			case GAME:
				_pszMetaBuffer = CFileManager::GetInstance()->LoadMetaFile("META_STAGE.txt");
				_next_token = _pszMetaBuffer;
			case CLEAR:
				stageHandler();
				break;
			case OVER:
				delete _pszMetaBuffer;// 소멸자에서 또 삭제함 메모리 누수 보고에 뜨길래 그냥 삭제한것
				_pszMetaBuffer = nullptr;
				_pScene = new CSceneOver;
				break;
			default:
				break;
			}
		}
		_pScene->Update();
		if (_go_to_terminate)
		{
			delete _pScene;
			_pScene = nullptr;
			break;
		}
	}
}

void CSceneManager::stageHandler(void)
{
	char* stageMetaFileName = strtok_s(_next_token, "\r\n\0",&_next_token);
	if (stageMetaFileName)
		_pScene = new CSceneGame{ stageMetaFileName };
	else
	{
		delete[] _pszMetaBuffer;
		_pszMetaBuffer = nullptr;
		_pScene = new CSceneClear;
	}
}
CSceneManager::CSceneManager()
	:_pScene{ nullptr }
{
	requestScene(TITLE);
}

CSceneManager::~CSceneManager()
{
	delete _pszMetaBuffer;
	_pszMetaBuffer = nullptr;
}

void CSceneManager::Destroy(void)
{
	delete _pSceneManager;
	_pSceneManager = nullptr;
}

void CSceneManager::requestScene(Scene scene)
{
	_isSceneChange = true;
	current_scene = scene;
}

void CSceneManager::going_to_terminate()
{
	_go_to_terminate = true;
}

