#pragma once

#include "MyList.h"
#include "BaseObject.h"
#include "PlayerObject.h"
#include "SceneBase.h"

class CObjectManager
{
public:
	static CObjectManager* GetInstance(void);
	void update();
	void render();
	void createPlayer();
	void createEnemy(const char symbol, const st_POS initialPos);
	bool createBullet(const ObjectType master_type, const st_POS object_pos, const BulletType bulletType, const BulletOrbitType orbitType, size_t* direction_index = nullptr);
	void notifyCollision();
	Scene isStageClear();
	const CEnemyType* getEnemyType(char symbol);
	const CBulletType* getBulletType(BulletType bulletType);
	const CPlayerType* getPlayerType();
private:
	static CObjectManager* _pObjectManager;
	MyList<CBaseObject*> _objectList;
	MyList<CEnemyType*> _enemyType;
	CPlayerType* _playerType;

	CBulletType _bulletTypeArr[BULLET_TYPE_NUM] = {
	{'#',PLAINSHOT,std::vector<st_POS>{{0,1}},1},
	{'@',FOURWAYSHOT,std::vector<st_POS>{{1,-1},{-1,-1},{-1,1},{1,1}},1},
	{'!',CLOCKWISE,std::vector<st_POS>{{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}},1},
	{ 'W',WIZARD,std::vector<st_POS>{{0,0}},1 }, //Çü½Ä»ó ¸¸µë.
	{'L',LASER,std::vector<st_POS>{{0,-1}},1} };
	void deleteAllobject();
	CObjectManager(void);
	~CObjectManager(void);
	static void Destroy(void);

	friend class CSceneGame;
};

