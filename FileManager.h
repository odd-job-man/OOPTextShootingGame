#pragma once
#include <cstdio>
#include "public_const.h"

class CFileManager
{
public:
	static CFileManager* GetInstance(void);
	void LoadScreenFile(const char* pszFileName);
	char* LoadMetaFile(const char* pszFileName);
private:
	static CFileManager* _pFileManager;
	int getFileSize(FILE* pFile);
	static void Destroy(void);
};
