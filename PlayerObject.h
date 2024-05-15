#pragma once
#include "BaseObject.h"
#include "ObjectManager.h"

class CPlayerObject : public CBaseObject, public CDamageList
{
public:
	void move(void) override;
	st_POS get_pos();
	bool isPlayerDead();
	static CPlayerObject* GetInstance();
	void handleCollision() override;
private:
	int _hp;
	static CPlayerObject* _pPlayerObject;
	void update(void) override;
	CPlayerObject(const CPlayerType* player_type);
	~CPlayerObject();

	friend class CObjectManager;


};
