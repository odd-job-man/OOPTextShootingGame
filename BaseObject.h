#pragma once
#include "vector"
#include "MyList.h"

enum BulletType { PLAINSHOT, FOURWAYSHOT, CLOCKWISE, WIZARD, LASER, BULLET_TYPE_NUM };
enum BulletOrbitType { LINEAR, KAMIKAZE, UPPER_RAY, FIRE_ALL_BULLET_FOURWAYSHOT, MAKE_ALL_KAMIKAZE, RESTORE, ORBIT_NUM };
enum ObjectType { PLAYER, ENEMY, BULLET, OBJECT_TYPE_NUM };

struct st_POS
{
	int _x;
	int _y;

	st_POS operator+ (const st_POS& other);
	st_POS operator+ (const st_POS& other) const;
	st_POS& operator+= (const st_POS& other);
	friend bool operator == (const st_POS& one, const st_POS& onoter);
	friend bool operator != (const st_POS& one, const st_POS& onother);
};

class CEnemyType
{
public:
	CEnemyType() = default;
	bool operator ==(const CEnemyType& other);
	char _symbol;
	int _hp;					// 적 HP
	BulletType _bulletType;
	BulletOrbitType _orbitType;
	st_POS* _path;
	int _movePattern; // _path 배열의 요소 갯수 

	CEnemyType(const char* pszFileName);
	~CEnemyType();
};

class CPlayerType
{
public:
	int _hp;
	st_POS _pos;
	BulletType _bulletType;
	BulletOrbitType _orbitType;
	char _symbol;
	CPlayerType(const char* pszFileName);
};
class CBulletType
{
public:
	char _symbol;
	BulletType _bulletType;
	std::vector<st_POS> _direction_vector_arr;
	int _damage;
};

class CBaseObject
{
public:
	CBaseObject(const st_POS pos, const ObjectType objectType, const BulletType bulletType, const BulletOrbitType orbitType, const char symbol);
	virtual ~CBaseObject();
	virtual void update(void) = 0;
	virtual void move(void) = 0;
	void render(void);
	static BulletType char_to_BulletType(const char* temp);
	static BulletOrbitType char_to_OrbitType(const char* temp);
	bool _visible;
	bool _isCollide;
	ObjectType _objectType;
	virtual void handleCollision();
protected:
	st_POS _pos;
	size_t _index_for_Circular_Pattern;
	BulletType _bulletType;
	BulletOrbitType _orbitType;
	char _symbol;

	friend class CObjectManager;
};


class CDamageList
{
public:
	MyList<int> _damage_list;
};
