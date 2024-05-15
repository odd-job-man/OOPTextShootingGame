#pragma once
#include "SceneBase.h"
class CSceneGame : public CSceneBase
{
public:
	CSceneGame(const char* pszFileName);
	~CSceneGame();
	void Update(void) override;
}; 