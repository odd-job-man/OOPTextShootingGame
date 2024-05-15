#pragma once
#include "BaseObject.h"
class CBulletObject : public CBaseObject
{
public:
	st_POS _direction_vector;
	BulletOrbitType _original_orbitType;
	ObjectType _master;
	CBulletObject(const st_POS object_pos, const BulletType bullet_type, const BulletOrbitType _orbitType, const ObjectType master, const size_t direct_index);
	void move() override;
	void update() override;
};

