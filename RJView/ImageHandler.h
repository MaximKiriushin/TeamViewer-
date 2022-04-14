#pragma once

#include <Windows.h>
#include "string"
#include "GdiPlusHelper.h"

#define PNG_MIN_QUAL 60
#define JPEG_MIN_QUAL 10
#define BMP_PATH L"image.bmp"
#define PNG_PATH L"image.png"
#define JPEG_PATH L"image.jpeg"

class ImageHandler
{
public:
	ImageHandler();//exception might be needed
	~ImageHandler();

	void captureImage();
	void saveImage(int qual);
	unsigned long getImageSize();
	std::wstring getImageName();
private:
	HWND hWin;
	HDC hdcWindow;
	HDC hdcMemDC;
	RECT rcClient;
	HBITMAP hbmScreen;

	HANDLE image;

	BITMAP bmpScreen;
	BITMAPFILEHEADER   bmfHeader;
	BITMAPINFOHEADER   bi;

	GdiPlusStartup gdiStartupInfo;

	std::wstring lastImageName;

	void initInfoHeader();
	void saveBmpFile(); 
	void savePngFile(); //exception might be needed
	void saveJpegFile(); //exception might be needed
	void saveJpegFile(unsigned long quality); //exception might be needed

	DWORD getBmpFileSize();
};