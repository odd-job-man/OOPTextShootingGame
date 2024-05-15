#pragma once
#include <Windows.h>

class CFrameManager
{
public:
	static CFrameManager* GetInstance();
	void set();
	bool isSleep();
	bool isExecute(const int execution_interval);
	void sleep();
	void updateFrame();
private:
	int _logic_fps;
	int _rendering_fps;
	int _tick;
	int _secondTick;
	int _oldFrameTick;
	static constexpr int TICK_PER_FRAME = 20;
	static constexpr int TICK_PER_SECONDS = 1000;
	static CFrameManager* _pFrameManager;
	static void Destroy();
	CFrameManager();
};
