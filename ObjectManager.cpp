#include "public_const.h"
#include "BaseObject.h"
#include "FileManager.h"
#include "ObjectManager.h"
#include "EnemyObject.h"
#include "PlayerObject.h"
#include "BulletObject.h"
#include "MyList.h"
#include "FrameManager.h"
#include "Windows.h"

#pragma warning (disable : 4715)

CObjectManager* CObjectManager::_pObjectManager = nullptr;

CObjectManager* CObjectManager::GetInstance(void)
{
	if (_pObjectManager == nullptr)
	{
		_pObjectManager = new CObjectManager;
		atexit(Destroy);
	}

	return _pObjectManager;
}

void CObjectManager::update()
{
	for (auto iter = _objectList.begin(); iter != _objectList.end(); ++iter)
		(*iter)->update();


	notifyCollision();

	for (auto iter = _objectList.begin(); iter != _objectList.end(); ++iter)
		(*iter)->handleCollision();

	for (auto iter = _objectList.begin(); iter != _objectList.end();)
	{
		if (!(*iter)->_visible)
		{
			auto& temp = *iter;
			delete temp;
			iter = _objectList.erase(iter);
		}
		else
			++iter;
	}
}

void CObjectManager::render()
{
	for (auto iter = _objectList.begin(); iter != _objectList.end(); ++iter)
		(*iter)->render();
}

void CObjectManager::createPlayer()
{
	_objectList.push_back(new CPlayerObject{ getPlayerType() });
}

void CObjectManager::createEnemy(const char symbol, const st_POS initialPos)
{
	if (symbol == 'W')
		_objectList.push_back(new CWizardObject{ initialPos });
	else
		_objectList.push_back(new CEnemyObject{ getEnemyType(symbol),initialPos });
}

bool CObjectManager::createBullet(const ObjectType master_type, const st_POS object_pos, const BulletType bulletType, const BulletOrbitType orbitType,size_t* direction_index)
{
	switch (bulletType)
	{
	case PLAINSHOT:
		if (CFrameManager::GetInstance()->isExecute(50))
		{
			_objectList.push_back(new CBulletObject{ object_pos,bulletType,orbitType,master_type,0 });
			return true;
		}
		break;
	case FOURWAYSHOT:
		if (CFrameManager::GetInstance()->isExecute(100))
		{
			for (size_t i = 0; i < CObjectManager::GetInstance()->getBulletType(bulletType)->_direction_vector_arr.size(); ++i)
				_objectList.push_back(new CBulletObject{ object_pos,bulletType,orbitType,master_type,i });
			return true;
		}
		break;
	case CLOCKWISE:
		if (CFrameManager::GetInstance()->isExecute(20))
		{
			_objectList.push_back(new CBulletObject{ object_pos,bulletType,orbitType,master_type,*direction_index });
			++(*direction_index);
			(*direction_index) %= CObjectManager::GetInstance()->getBulletType(bulletType)->_direction_vector_arr.size();
			return true;
		}
		break;
	case LASER:
		if (master_type == PLAYER && CFrameManager::GetInstance()->isExecute(20))
		{
			int max = 0;
			for (auto iter = _objectList.begin(); iter != _objectList.end(); ++iter)
			{
				do
				{
					if ((*iter)->_pos._x != object_pos._x)
						break;

					if ((*iter)->_objectType != ENEMY)
						break;

					if ((*iter)->_pos._y >= object_pos._y)
						break;

					if ((*iter)->_pos._y > max)
						max = (*iter)->_pos._y;
				} while (false);
			}
			if (max == 0)
				max = 1;

			int y = object_pos._y - 1;
			while (max < y)
			{
				_objectList.push_back(new CBulletObject{ st_POS{object_pos._x,y},LASER,UPPER_RAY,master_type,*direction_index });
				--y;
			}
			return true;
		}
		break;
	case WIZARD:
		if (orbitType == FIRE_ALL_BULLET_FOURWAYSHOT && CFrameManager::GetInstance()->isExecute(100))
		{
			for (auto iter = _objectList.begin(); iter != _objectList.end() && _objectList._size <= 200000; ++iter)
			{
				auto* temp = iter._node->_data;
				if (temp->_objectType == BULLET && static_cast<CBulletObject*>(temp)->_master == ENEMY)
				{
					for (size_t i = 0; i < CObjectManager::GetInstance()->getBulletType(FOURWAYSHOT)->_direction_vector_arr.size(); ++i)
						_objectList.push_back(new CBulletObject{ temp->_pos,FOURWAYSHOT,LINEAR,BULLET,i });
				}
			}

			return true;
		}
		else if (orbitType == MAKE_ALL_KAMIKAZE && CFrameManager::GetInstance()->isExecute(49))
		{
			for (auto iter = _objectList.begin(); iter != _objectList.end(); ++iter)
			{
				auto* temp = iter._node->_data;
				if (temp->_objectType == BULLET && static_cast<CBulletObject*>(temp)->_master != PLAYER)
				{
					static_cast<CBulletObject*>(temp)->_original_orbitType = temp->_orbitType; // 원래 orbitType 백업
					temp->_orbitType = KAMIKAZE;
				}
			}
			return true;
		}
		else if (orbitType == RESTORE && CFrameManager::GetInstance()->isExecute(51))
		{
			for (auto iter = _objectList.begin(); iter != _objectList.end(); ++iter)
			{
				auto* temp = iter._node->_data;
				if (temp->_objectType == BULLET && static_cast<CBulletObject*>(temp)->_original_orbitType != temp->_orbitType)
					temp->_orbitType = static_cast<CBulletObject*>(temp)->_original_orbitType; // 복구
			}
			return true;
		}
	default:
		break;
	}
	return false; // 맞는 타입이 없거나 공격을 실행할 타이밍이 아님.
}

CObjectManager::CObjectManager()
{
	char* enemyListFileBuffer = CFileManager::GetInstance()->LoadMetaFile("ENEMY.txt");
	char* next_token = enemyListFileBuffer;

	char* listTemp = strtok_s(next_token, ",\r\n", &next_token);
	_enemyType.push_back(new CEnemyType{ listTemp });

	while (( listTemp = strtok_s(next_token, ",\r\n", &next_token)) != nullptr)
		_enemyType.push_back(new CEnemyType{ listTemp });

	_playerType = new CPlayerType{ "Player.txt" };
	delete[] enemyListFileBuffer;
}

CObjectManager::~CObjectManager(void)
{
	delete _playerType;
	for (int i = 0; i < BULLET_TYPE_NUM; ++i)
		_bulletTypeArr[i]._direction_vector_arr.clear();

	for (auto iter = _enemyType.begin(); iter != _enemyType.end();)
	{
		delete* iter;
		iter = _enemyType.erase(iter);
	}
	deleteAllobject();
}

void CObjectManager::notifyCollision()
{
	// 적이나 총알이 발사한 총알이 플레이어한테 맞앚을때의 충돌처리
	st_POS player_pos = CPlayerObject::_pPlayerObject->get_pos();
	for (auto iter = _objectList.begin(); iter != _objectList.end(); ++iter)
	{
		auto& temp = iter._node->_data;
		do
		{
			if (temp->_objectType != BULLET)
				break;
			if (static_cast<CBulletObject*>(temp)->_master == PLAYER)
				break;
			if (player_pos != temp->_pos)
				break;
			CPlayerObject::_pPlayerObject->_isCollide = true;
			CPlayerObject::_pPlayerObject->_damage_list.push_back(CObjectManager::getBulletType(temp->_bulletType)->_damage);
			temp->_isCollide = true;
		} while (false);
	}

	// 플레이어 총알과 적에 대한 충돌처리
	for (auto iter = _objectList.begin(); iter != _objectList.end(); ++iter)
	{
		auto& player_bullet = iter._node->_data;
		do
		{
			// 플레이어 총알 골라내기
			if (player_bullet->_objectType != BULLET)
				break;
			if (static_cast<CBulletObject*>(player_bullet)->_master != PLAYER)
				break;
			// 플레이어의 총알과 위치가 같은 적 찾기
			for (auto iter2 = _objectList.begin(); iter2 != _objectList.end(); ++iter2)
			{
				do
				{
					if (iter2._node->_data->_objectType != ENEMY)
						break;
						
					auto* enemy = static_cast<CEnemyObject*>(iter2._node->_data);
					if (player_bullet->_pos != enemy->_pos)
						break;

					// 플레이어의 총알과 위치가 같은 적을 발견
					player_bullet->_isCollide = true;
					enemy->_damage_list.push_back(CObjectManager::getBulletType(player_bullet->_bulletType)->_damage);
					enemy->_isCollide = true;
				} while (false);
			}
		} while (false);
	}
}

Scene CObjectManager::isStageClear()
{
	bool is_player_wantTo_quit = false;
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8001)
		is_player_wantTo_quit = true;

	if (_objectList._size == 1 && CPlayerObject::_pPlayerObject != nullptr)
		return CLEAR;
	else if (CPlayerObject::_pPlayerObject == nullptr || is_player_wantTo_quit == true)
		return OVER;
	else
		return NOCHANGE;
}

void CObjectManager::deleteAllobject()
{
	for (auto iter = _objectList.begin(); iter != _objectList.end();)
	{
		delete* iter;
		iter = _objectList.erase(iter);
	}
}

const CEnemyType* CObjectManager::getEnemyType(char symbol)
{
	for (auto iter = _enemyType.begin(); iter != _enemyType.end(); ++iter)
	{
		if ((*iter)->_symbol == symbol)
			return *iter;
	}

	return nullptr;
}

const CBulletType* CObjectManager::getBulletType(BulletType bulletType)
{
	return _bulletTypeArr + bulletType;
}

const CPlayerType* CObjectManager::getPlayerType()
{
	return _playerType;
}


void CObjectManager::Destroy(void)
{
	delete _pObjectManager;
	_pObjectManager = nullptr;
}
