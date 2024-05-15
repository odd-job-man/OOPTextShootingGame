#include <iostream>
#include <windows.h>
#include <stdio.h> 
#include "SceneBase.h"
#include "Scenemanager.h"
#include "FileManager.h"
#include "ScreenManager.h"

int main()
 {
	cs_Initial();
	CSceneManager::GetInstance()->run();
	return 0;
}
