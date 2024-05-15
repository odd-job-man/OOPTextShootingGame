#include "BaseObject.h"
#include "PlayerObject.h"
#include "ScreenManager.h"
#include "FrameManager.h"
#include <Windows.h>

CPlayerObject* CPlayerObject::_pPlayerObject = nullptr;

CPlayerObject::CPlayerObject(const CPlayerType* player_type)
	:_hp{ player_type->_hp }, CBaseObject{ player_type->_pos,PLAYER,player_type->_bulletType,player_type->_orbitType,player_type->_symbol } 
{
	_pPlayerObject = this;
}

CPlayerObject::~CPlayerObject()
{
	_pPlayerObject = nullptr;
}

void CPlayerObject::handleCollision()
{
	if (!_isCollide)
		return;

	_isCollide = false;
	for (auto iter = _damage_list.begin(); iter != _damage_list.end();)
	{
		_hp -= iter._node->_data;
		iter = _damage_list.erase(iter); // iter가 가리키는게 int라서 따로 delete *iter는 필요없음.
	}

	if (_hp <= 0)
		_visible = false;
}


void CPlayerObject::update(void)
{
	move();
	if (GetAsyncKeyState(VK_CONTROL))
		CObjectManager::GetInstance()->createBullet(PLAYER, _pos, _bulletType, _orbitType, &_index_for_Circular_Pattern);

}

void CPlayerObject::move(void)
{
	if (!CFrameManager::GetInstance()->isExecute(2))
		return;
	if (GetAsyncKeyState(VK_LEFT) & 0x8001)
		--_pos._x;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8001)
		++_pos._x;
	if (GetAsyncKeyState(VK_UP) & 0x8001)
		--_pos._y;
	if (GetAsyncKeyState(VK_DOWN) & 0x8001)
		++_pos._y;

	// 이동반경 제한
	_pos._x= max(_pos._x, 1);
	_pos._x= min(_pos._x, 233);
	_pos._y = max(_pos._y, 1);
	_pos._y = min(_pos._y, 60);
}

st_POS CPlayerObject::get_pos()
{
	return st_POS{_pos._x,_pos._y};
}

bool CPlayerObject::isPlayerDead()
{
	return !_visible;
}

CPlayerObject* CPlayerObject::GetInstance()
{
	if (_pPlayerObject)
		return _pPlayerObject;
	else
		return nullptr;
		
}


