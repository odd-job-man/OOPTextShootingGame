#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "FileManager.h"
#include "ScreenManager.h"

#define dfSCREEN_WIDTH	237		// 콘솔 가로 80칸 + NULL
#define dfSCREEN_HEIGHT	62		// 콘솔 세로 25칸 + NULL
#define SCREENSIZE ((dfSCREEN_HEIGHT) * (dfSCREEN_WIDTH))

CFileManager* CFileManager::_pFileManager = nullptr;

CFileManager* CFileManager::GetInstance(void)
{
	if (_pFileManager == nullptr)
	{
		_pFileManager = new CFileManager;
		atexit(Destroy);
	}
	return _pFileManager;
}


// 스크린사이즈의 버퍼에 스테이지, 타이틀, 게임오버스크린등을 담아줍니다.
void CFileManager::LoadScreenFile(const char* pszFileName)
{

	FILE* pFile;
	fopen_s(&pFile, pszFileName, "rb");
	if (!pFile)
	{
		printf("%d\n", GetLastError());
		return;
	}

	int iFileSize = getFileSize(pFile);
	char* FileBuffer = new char[iFileSize];
	if (!fread_s(FileBuffer, iFileSize, iFileSize, 1, pFile))
	{
		printf("%d\n", GetLastError());
		return;
	}

	char* next_token = FileBuffer;

	CScreenManager::SCREENPOINTER pszRow = CScreenManager::GetInstance()->getScreen();
	char* tempBuffer = strtok_s(next_token, "\r\n", &next_token);
	strcpy_s(pszRow[0], _countof(pszRow[0]), tempBuffer);

	for (int i = 1; i < dfSCREEN_HEIGHT && tempBuffer; ++i)
	{
		tempBuffer = strtok_s(next_token, "\r\n", &next_token);
		strcpy_s(pszRow[i], _countof(pszRow[0]), tempBuffer);
	}
	fclose(pFile);
	delete[] FileBuffer;
}

//metaBuffer의 사이즈가 얼마가 필요할지 모르기때문에 동적할당된 주소를 돌려주므로 이 주소를 가진 클래스의 소멸자에서 파괴하세요.
// 또한 바이너리 모드로 읽어서 \r\n이 삽입되어있으니 이 버퍼를 사용하는 측에서 적당히 파싱해서 읽으셔야합니다.
// malloc 해서 넘기는것이니 꼭 free하세요.:
char* CFileManager::LoadMetaFile(const char* pszFileName)
{
	FILE* pFile;
	fopen_s(&pFile, pszFileName, "rb");
	if (!pFile)
	{
		printf("%d\n", GetLastError());
		return nullptr;
	}
	int iFileSize = getFileSize(pFile);
	char* FileBuffer = new char[iFileSize + 1];
	FileBuffer[iFileSize] = 0;
	if (!fread_s(FileBuffer, iFileSize, iFileSize, 1, pFile))
	{
		printf("%d\n", GetLastError());
		return nullptr;
	}

	fclose(pFile);
	return FileBuffer;
}

int CFileManager::getFileSize(FILE* pFile)
{
	fseek(pFile, 0, SEEK_END);
	int iFileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	return iFileSize;
}


void CFileManager::Destroy(void)
{
	delete _pFileManager;
	_pFileManager = nullptr;
}
