#include <stdlib.h>
#include <Windows.h>
#pragma comment(lib,"winmm.lib")
#include "FrameManager.h"

CFrameManager* CFrameManager::_pFrameManager = nullptr;

CFrameManager* CFrameManager::GetInstance()
{
	if (_pFrameManager == nullptr)
	{
		_pFrameManager = new CFrameManager();
		atexit(Destroy);
	}
	return _pFrameManager;
}

void CFrameManager::set()
{
	_oldFrameTick = timeGetTime();
	_secondTick = _oldFrameTick;
}

bool CFrameManager::isSleep()
{
	_tick = timeGetTime();
	if (_tick - _oldFrameTick < TICK_PER_FRAME)
		return true;
	
	_oldFrameTick += TICK_PER_FRAME;
	_logic_fps = (++_logic_fps) % TICK_PER_SECONDS;
	return false;
}

bool CFrameManager::isExecute(const int execution_interval)
{
	return (_logic_fps % execution_interval) == 0;
}

void CFrameManager::sleep()
{
	Sleep(TICK_PER_FRAME - (_tick - _oldFrameTick));
}

void CFrameManager::updateFrame()
{
	_logic_fps = (++_logic_fps) % TICK_PER_SECONDS;
	_rendering_fps = (++_rendering_fps) % TICK_PER_SECONDS;
	_oldFrameTick += TICK_PER_FRAME;
}

void CFrameManager::Destroy()
{
	timeEndPeriod(1);
	delete _pFrameManager;
	_pFrameManager = nullptr;
}

CFrameManager::CFrameManager()
	:_logic_fps{ 0 }, _rendering_fps{ 0 }, _tick{ 0 }, _oldFrameTick{ 0 } 
{
	timeBeginPeriod(1);
};
