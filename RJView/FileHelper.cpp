#include <Windows.h>
#include "FileHelper.h"
#include <iostream>
void getFile(imageInfo info, char** buff)
{
	HANDLE hFile = NULL;
	DWORD size = 0;
	
	*buff = new char[info.imageLength];
	memset(*buff, 0, info.imageLength);
	std::cout << info.imageLength << std::endl;
	hFile = CreateFile(info.name.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (!ReadFile(hFile, *buff, info.imageLength, NULL, NULL))
	{
		std::cout << "err";
		//exception
	}
	CloseHandle(hFile);
}

void saveImage(char* imageContent, imageInfo info)
{
	saveImage(imageContent, info, 0);
}

void saveImage(char* imageContent, imageInfo info, int part)
{
	OVERLAPPED overlap = {0};
	overlap.Offset = CHUNK_PART*part;
	HANDLE hFile = NULL;
	DWORD dwBytesWritten = 0;
	//hFile = CreateFile(info.name.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//hFile = CreateFile(L"test.jpeg", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	hFile = CreateFile(L"test.bmp", GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, imageContent, info.imageLength, &dwBytesWritten, &overlap);
	CloseHandle(hFile);
}

imageInfo praseImageInfo(std::string infoStr)
{
	imageInfo info;
	std::string temp;
	temp = infoStr.substr(0, IMAGE_NAME_MAX_LEN);
	std::replace(temp.begin(), temp.end(), '0', (char)(0));
	info.name = std::wstring(temp.begin(),temp.end());
	info.imageLength = stoi(infoStr.substr(IMAGE_NAME_MAX_LEN, IMAGE_MAX_LEN));
	return info;
}