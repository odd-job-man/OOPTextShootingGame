#pragma once
#include "BaseObject.h"

class CEnemyObject : public CBaseObject, public CDamageList
{
public:
	void handleCollision() override;
protected:
	void update() override;
	void move(void) override;
	CEnemyObject(const CEnemyType* enemyType, const st_POS initialPos);
private:
	int _hp;
	int _iteration_count;
	st_POS _initial_pos;
	friend class CObjectManager;
};

class CWizardObject : public CEnemyObject
{
public:
	CWizardObject(const st_POS object_pos);
	void update() override;
	bool _need_to_restore = false;
};
