#include "BaseObject.h"
#include "EnemyObject.h"
#include "ObjectManager.h"
#include "ScreenManager.h"
#include "FrameManager.h"

CEnemyObject::CEnemyObject(const CEnemyType* enemyType, const st_POS initialPos)
	:CBaseObject{ initialPos,ENEMY,enemyType->_bulletType,enemyType->_orbitType,enemyType->_symbol }, _iteration_count{ 0 }
{
	_initial_pos = _pos;
	_hp = enemyType->_hp;
}

void CEnemyObject::update(void) 
{
	move();
	CObjectManager::GetInstance()->createBullet(ENEMY, _pos, _bulletType, _orbitType,&_index_for_Circular_Pattern);
}

void CEnemyObject::move()
{
	auto&& temp = CObjectManager::GetInstance()->getEnemyType(_symbol);
	if (CFrameManager::GetInstance()->isExecute(50))
	{
		_pos = _initial_pos + temp->_path[_iteration_count];
		_iteration_count = (++_iteration_count) % temp->_movePattern;
	}
}

void CEnemyObject::handleCollision()
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

CWizardObject::CWizardObject(const st_POS object_pos)
	:CEnemyObject{ CObjectManager::GetInstance()->getEnemyType('W'),object_pos }
{
}

void CWizardObject::update()
{
	move();
	CObjectManager::GetInstance()->createBullet(ENEMY, _pos, WIZARD, FIRE_ALL_BULLET_FOURWAYSHOT, &_index_for_Circular_Pattern);
	if (_need_to_restore)
		_need_to_restore = !CObjectManager::GetInstance()->createBullet(ENEMY, _pos, WIZARD, RESTORE, &_index_for_Circular_Pattern);
	else
		_need_to_restore = CObjectManager::GetInstance()->createBullet(ENEMY, _pos, WIZARD, MAKE_ALL_KAMIKAZE, &_index_for_Circular_Pattern);
}
