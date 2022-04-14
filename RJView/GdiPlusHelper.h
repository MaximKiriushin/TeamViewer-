#pragma once

#include <Windows.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

using namespace Gdiplus;

struct GdiPlusStatup
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
} typedef GdiPlusStartup;

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);