#include "BaseObject.h"
#include "BulletObject.h"
#include "ObjectManager.h"
#include "PlayerObject.h"
#include "FrameManager.h"

CBulletObject::CBulletObject(const st_POS object_pos, const BulletType bullet_type, const BulletOrbitType orbitType, const ObjectType master, const size_t direct_index = 0)
	:CBaseObject{ object_pos,BULLET,bullet_type,orbitType,CObjectManager::GetInstance()->getBulletType(bullet_type)->_symbol }
{
	auto temp = CObjectManager::GetInstance()->getBulletType(bullet_type);
	_direction_vector = CObjectManager::GetInstance()->getBulletType(bullet_type)->_direction_vector_arr[direct_index];
	_orbitType = orbitType;
	_original_orbitType = orbitType;
	_master = master;
}


void CBulletObject::move()
{
	switch (_orbitType)
	{
	case LINEAR:
		if (!CFrameManager::GetInstance()->isExecute(2))
			return;
	case UPPER_RAY:
		break;
	case KAMIKAZE:
	{
		auto* temp = CPlayerObject::GetInstance();
		if (temp)
		{
			st_POS player_pos = temp->get_pos();
			int dx = player_pos._x - _pos._x;
			int dy = player_pos._y - _pos._y;

			if (dx == 0)
				dx = 1;
			else
				dx /= abs(dx);

			if (dy == 0) 
				dy = 1;
			else
				dy /= abs(dy);

			_direction_vector = st_POS{ dx,dy };
		}
		break;
	}
	case MAKE_ALL_KAMIKAZE:
	default:
		return;
	}

	_pos += _direction_vector;

	switch (_orbitType)
	{
	case LINEAR:
	case UPPER_RAY:
		if (_pos._x < 2 || _pos._x >= 234 || _pos._y < 1 || _pos._y > 60)
			_visible = false;
		break;
	case KAMIKAZE:
		if (_pos._x <= 2 || _pos._x >= 234 || _pos._y <= 2 || _pos._y >= 60)
			_visible = false;
		break;
	case FIRE_ALL_BULLET_FOURWAYSHOT:
	case MAKE_ALL_KAMIKAZE:
	default:
		return;
	}
}

void CBulletObject::update()
{
	move();
}
