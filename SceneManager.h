#pragma once
#include "SceneBase.h"

class CSceneManager
{
public:
	Scene current_scene;
	static CSceneManager* GetInstance(void);
	void run();
	void requestScene(Scene scene);
	void going_to_terminate();
private:
	bool _go_to_terminate = false;
	char* _pszMetaBuffer;
	char* _next_token;
	bool _isSceneChange;
	CSceneBase* _pScene;
	static CSceneManager* _pSceneManager;
	void stageHandler(void);
	CSceneManager();
	
	~CSceneManager();
	static void Destroy(void);
};
