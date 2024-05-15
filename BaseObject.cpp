#include <map>
#include "BaseObject.h"
#include "FileManager.h"
#include "ScreenManager.h"


CEnemyType::CEnemyType(const char* pszFileName)
{
	char* enemyMetaFileBuffer = CFileManager::GetInstance()->LoadMetaFile(pszFileName);
	char* next_token = enemyMetaFileBuffer;
	char* temp = strtok_s(next_token, ",\r\n", &next_token);
	_symbol = temp[0];
	
	temp = strtok_s(next_token, ",\r\n", &next_token);
	_hp = atoi(temp);

	temp = strtok_s(next_token, ",\r\n", &next_token);
	_bulletType = CBaseObject::char_to_BulletType(temp);
	temp = strtok_s(next_token, ",\r\n", &next_token);
	_orbitType = CBaseObject::char_to_OrbitType(temp);

	_movePattern = atoi(strtok_s(next_token, ",\r\n", &next_token));
	_path = new st_POS[_movePattern];
	
	for (int i = 0; i < _movePattern; ++i)
	{
		_path[i]._x =  atoi(strtok_s(nullptr, ",\r\n", &next_token));
		_path[i]._y = atoi(strtok_s(nullptr, ",\r\n", &next_token));
	}
	delete enemyMetaFileBuffer;
}

CEnemyType::~CEnemyType()
{
	delete[] _path;
	_path = nullptr;
}

bool CEnemyType::operator==(const CEnemyType& other)
{
	return (_hp == other._hp) && (_bulletType == other._bulletType) && (_path
	== other._path) && (_movePattern == other._movePattern) && (_symbol == other._symbol);
}

CBaseObject::CBaseObject(const st_POS pos, const ObjectType objectType, const BulletType bulletType, const BulletOrbitType orbitType, const char symbol)
	:_pos{ pos }, _objectType{ objectType }, _bulletType{ bulletType },
	_symbol{ symbol }, _visible{ true }, _orbitType{ orbitType }, _index_for_Circular_Pattern{ 0 }, _isCollide{ false } {}

CBaseObject::~CBaseObject()
{
}


void CBaseObject::render(void)
{
	CScreenManager::GetInstance()->Sprite_Draw(_pos, _symbol);
}

BulletType CBaseObject::char_to_BulletType(const char* temp)
{
	switch (temp[0])
	{
		case 'P':
		if (!strcmp(temp, "PLAINSHOT"))
			return PLAINSHOT;
		case 'F':
		if (!strcmp(temp, "FOURWAYSHOT"))
			return FOURWAYSHOT;
		case 'C':
		if (!strcmp(temp, "CLOCKWISE"))
			return CLOCKWISE;
		case 'W':
		if (!strcmp(temp, "WIZARD"))
			return WIZARD;
		case 'L':
			if (!strcmp(temp, "LASER"))
			return LASER;
		default:
			return BULLET_TYPE_NUM;
	}
}

BulletOrbitType CBaseObject::char_to_OrbitType(const char* temp)
{

	switch (temp[0])
	{
	case 'L':
		if (!strcmp(temp, "LINEAR"))
			 return LINEAR;
	case 'K':
		if (!strcmp(temp, "KAMIKAZE"))
			return KAMIKAZE;
	case 'U':
		if (!strcmp(temp, "UPPER_RAY"))
			return UPPER_RAY;
	case 'F':
		if (!strcmp(temp, "FIRE_ALL_BULLET_FOURWAYSHOT"))
			return FIRE_ALL_BULLET_FOURWAYSHOT;
	case 'M':
		if (!strcmp(temp, "MAKE_ALL_KAMIKAZE"))
			return MAKE_ALL_KAMIKAZE;
	case 'R':
		if (!strcmp(temp, "RESTORE"))
			return RESTORE;
	default:
		return ORBIT_NUM;
	}
}

// 총알은 오버라이딩 안하고 이걸 그대로 쓸겁니다.
void CBaseObject::handleCollision()
{
	if (_isCollide)
		_visible = false;
}

st_POS st_POS::operator+(const st_POS& other) 
{
	return st_POS{ _x + other._x,_y + other._y };
}

st_POS st_POS::operator+(const st_POS& other) const
{
	return st_POS{ _x + other._x,_y + other._y };
}

st_POS& st_POS::operator+=(const st_POS& other)
{
	_x += other._x;
	_y += other._y;
	return *this;
}

CPlayerType::CPlayerType(const char* pszFileName)
{
	char* Buffer = CFileManager::GetInstance()->LoadMetaFile(pszFileName);
	char* next_token = Buffer;
	char* temp = strtok_s(next_token, ",\r\n", &next_token);
	_symbol = temp[0];
	
	temp = strtok_s(next_token, ",\r\n", &next_token);
	_hp = atoi(temp);

	temp = strtok_s(next_token, ",\r\n", &next_token);
	_bulletType = CBaseObject::char_to_BulletType(temp);

	temp = strtok_s(next_token, ",\r\n", &next_token);
	_orbitType = CBaseObject::char_to_OrbitType(temp);

	temp = strtok_s(next_token, ",\r\n", &next_token);
	_pos._x = atoi(temp);
	temp = strtok_s(next_token, ",\r\n", &next_token);
	_pos._y = atoi(temp);

	delete[] Buffer;
}

bool operator==(const st_POS& one, const st_POS& onoter)
{
	return (one._x == onoter._x && one._y == onoter._y);
}

bool operator!=(const st_POS& one, const st_POS& onother)
{
	return !(one == onother);
}
