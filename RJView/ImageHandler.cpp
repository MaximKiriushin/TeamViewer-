#include "ImageHandler.h"

ImageHandler::ImageHandler()
{
    //retrive the handle of the desktop window
    this->hWin = GetDesktopWindow();
    //retrive the device context for the entire screen
    this->hdcWindow = GetDC(NULL);
    //creates a compatible dc to use in the bitBlt
    this->hdcMemDC = CreateCompatibleDC(this->hdcWindow);
    if (!hdcMemDC);
    //get the client rectangle for size calaculations
    GetClientRect(hWin, &this->rcClient);
    //creates a compatible bitmap
    this->hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
    if (!hbmScreen);
    //selects the compatible bitmap into the comptible dc
    SelectObject(hdcMemDC, hbmScreen);

    ZeroMemory(&bmpScreen, sizeof(BITMAP));
    ZeroMemory(&bmfHeader, sizeof(BITMAPFILEHEADER));
    ZeroMemory(&bi, sizeof(BITMAPINFOHEADER));

    captureImage();

    this->image = NULL;
}

ImageHandler::~ImageHandler()
{
    DeleteObject(this->hbmScreen);
    DeleteObject(this->hdcMemDC);
    ReleaseDC(this->hWin, this->hdcWindow);
}

void ImageHandler::captureImage()
{
    GlobalFree(image);
    //Bit block transfer into our compatible memory DC.
    BitBlt(this->hdcMemDC, 0, 0, this->rcClient.right - this->rcClient.left, this->rcClient.bottom - this->rcClient.top, this->hdcWindow, 0, 0, SRCCOPY);
}

void ImageHandler::saveImage(int quality)
{
    if (quality == 100)
    {
        saveBmpFile();
        this->lastImageName = BMP_PATH;
    }
    else if (quality > PNG_MIN_QUAL)
    {
        this->savePngFile();
        this->lastImageName = PNG_PATH;
    }
    else
    {
        this->saveJpegFile(quality);
        this->lastImageName = JPEG_PATH;
    }
}

unsigned long ImageHandler::getImageSize()
{
    std::wstring name = this->getImageName();
    return GetCompressedFileSize(name.c_str(), NULL);
}

std::wstring ImageHandler::getImageName()
{
    return this->lastImageName;
}

void ImageHandler::initInfoHeader()
{
    this->bi.biSize = sizeof(BITMAPINFOHEADER);
    this->bi.biWidth = this->bmpScreen.bmWidth;
    this->bi.biHeight = this->bmpScreen.bmHeight;
    this->bi.biPlanes = 1;
    this->bi.biBitCount = 32;
    this->bi.biCompression = BI_RGB;
    this->bi.biSizeImage = 0;
    this->bi.biXPelsPerMeter = 0;
    this->bi.biYPelsPerMeter = 0;
    this->bi.biClrUsed = 0;
    this->bi.biClrImportant = 0;
}

void ImageHandler::saveBmpFile()
{
    DWORD dwBmpSize = 0;
    char* lpbitmap = NULL;
    HANDLE hFile = NULL;
    DWORD dwBytesWritten = 0;
    DWORD dwSizeofDIB = 0;

    GetObject(this->hbmScreen, sizeof(BITMAP), &this->bmpScreen);
    this->initInfoHeader();
    this->image = GlobalAlloc(GHND, this->getBmpFileSize());
    lpbitmap = (char*)GlobalLock(this->image);
    GetDIBits(this->hdcWindow, this->hbmScreen, 0, (UINT)this->bmpScreen.bmHeight, lpbitmap, (BITMAPINFO*)&this->bi, DIB_RGB_COLORS);

    dwBmpSize = this->getBmpFileSize();

    hFile = CreateFile(L"image.bmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    this->bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
    this->bmfHeader.bfSize = dwSizeofDIB;
    this->bmfHeader.bfType = 0x4D42;

    WriteFile(hFile, (LPSTR)&this->bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)&this->bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
    WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);
    GlobalUnlock(this->image);
    GlobalFree(this->image);
    CloseHandle(hFile);
}

void ImageHandler::savePngFile()
{
    int status = 0;
    Gdiplus::GdiplusStartup(&gdiStartupInfo.gdiplusToken, &gdiStartupInfo.gdiplusStartupInput, NULL);

    CLSID   encoderClsid;
    Gdiplus::Status  stat;
    Bitmap* p_bmp = Bitmap::FromHBITMAP(this->hbmScreen, NULL);

    // Get the CLSID of the PNG encoder.
    GetEncoderClsid(L"image/png", &encoderClsid);

    stat = p_bmp->Save(PNG_PATH, &encoderClsid, NULL);

    if (stat == Gdiplus::Ok);

    delete p_bmp;
    Gdiplus::GdiplusShutdown(gdiStartupInfo.gdiplusToken);
}

void ImageHandler::saveJpegFile()
{
    this->saveJpegFile(50);
}

void ImageHandler::saveJpegFile(unsigned long qual)
{
    Gdiplus::GdiplusStartup(&gdiStartupInfo.gdiplusToken, &gdiStartupInfo.gdiplusStartupInput, NULL);

    CLSID             encoderClsid;
    Gdiplus::EncoderParameters encoderParameters;
    ULONG             quality;
    Gdiplus::Status            stat;
    int status = 0;
    // Get an image from the disk.
    Bitmap* p_bmp = Bitmap::FromHBITMAP(this->hbmScreen, NULL);

    // Get the CLSID of the JPEG encoder.
    GetEncoderClsid(L"image/jpeg", &encoderClsid);

    // Before we call Image::Save, we must initialize an
    // EncoderParameters object. The EncoderParameters object
    // has an array of EncoderParameter objects. In this
    // case, there is only one EncoderParameter object in the array.
    // The one EncoderParameter object has an array of values.
    // In this case, there is only one value (of type ULONG)
    // in the array. We will let this value vary from 0 to 100.

    encoderParameters.Count = 1;
    encoderParameters.Parameter[0].Guid = Gdiplus::EncoderQuality;
    encoderParameters.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
    encoderParameters.Parameter[0].NumberOfValues = 1;

    // Save the image as a JPEG with quality level 0.
    quality = qual;
    encoderParameters.Parameter[0].Value = &quality;
    stat = p_bmp->Save(JPEG_PATH, &encoderClsid, &encoderParameters);

    if (stat == Gdiplus::Ok);
    delete p_bmp;
    Gdiplus::GdiplusShutdown(gdiStartupInfo.gdiplusToken);
}

DWORD ImageHandler::getBmpFileSize()
{
    return ((this->bmpScreen.bmWidth * this->bi.biBitCount + 31) / 32) * 4 * this->bmpScreen.bmHeight;;
}