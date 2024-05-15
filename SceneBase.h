#pragma once
#include "public_const.h"

enum Scene { TITLE, GAME, CLEAR, OVER, NOCHANGE };

class CSceneBase
{
public:
	virtual void Update() = 0;
	virtual ~CSceneBase() {};
};
