#pragma once

#include  <string>
#include <algorithm>
#include "CommunicatorHelper.h"

#define CHUNK_PART 50000
#define CHUNK_ID_LENGTH 4
#define TEST_FILE L"test.txt"
struct imageInfo {
	int imageLength = 0;
	std::wstring name = std::wstring(L"");
}typedef imageInfo;


void getFile(imageInfo info, char** buff);
void saveImage(char* imageContent, imageInfo info);
void saveImage(char* imageContent, imageInfo info, int part);
imageInfo praseImageInfo(std::string infoStr);