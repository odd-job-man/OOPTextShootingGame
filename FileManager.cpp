#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "FileManager.h"
#include "ScreenManager.h"

#define dfSCREEN_WIDTH	237		// �ܼ� ���� 80ĭ + NULL
#define dfSCREEN_HEIGHT	62		// �ܼ� ���� 25ĭ + NULL
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


// ��ũ���������� ���ۿ� ��������, Ÿ��Ʋ, ���ӿ�����ũ������ ����ݴϴ�.
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

//metaBuffer�� ����� �󸶰� �ʿ����� �𸣱⶧���� �����Ҵ�� �ּҸ� �����ֹǷ� �� �ּҸ� ���� Ŭ������ �Ҹ��ڿ��� �ı��ϼ���.
// ���� ���̳ʸ� ���� �о \r\n�� ���ԵǾ������� �� ���۸� ����ϴ� ������ ������ �Ľ��ؼ� �����ž��մϴ�.
// malloc �ؼ� �ѱ�°��̴� �� free�ϼ���.:
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
