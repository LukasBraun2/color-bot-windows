/*
important side note, 759 and 509 are the window width and height, will certainly change if scaling to larger images
*/

#include <windows.h>
#include <fstream>
#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <cstdlib>
#include <winuser.h>


void CaptureScreenToBitmap(const char* filename) {    // Get the screen dimensions
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    HWND hwnd = FindWindow(NULL, "Conquest");
    if (!hwnd) {
        std::cerr << "Window not found!" << std::endl;
        return;
    }

   // RECT rect;
    //int screenWidth = 0;
    //int screenHeight = 0;
    RECT rect;
    int width, height = 0;
    if(GetWindowRect(hwnd, &rect))
    {
      width = rect.right - rect.left-293;
      height = rect.bottom - rect.top-173;
    }
    //screenWidth -= 1;
    //screenHeight -= 1;
    std::cout << " " << width << " " << height << std::endl;

    //std::cout << screenWidth << " " << screenHeight << std::endl;
    
    // Create a device context for the entire screen

    HDC hScreenDC = GetDC(hwnd);

    // Create a compatible device context
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

    // Create a bitmap
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);

    // Select the bitmap into the memory device context
    SelectObject(hMemoryDC, hBitmap);

    // Copy the screen content to the bitmap
    BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);

    // Get bitmap information
    BITMAPINFOHEADER bmiHeader;
    bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmiHeader.biWidth = width;
    bmiHeader.biHeight = -height; // Negative height for top-down bitmap
    bmiHeader.biPlanes = 1;
    bmiHeader.biBitCount = 24; // 24-bit color depth
    bmiHeader.biCompression = BI_RGB;
    bmiHeader.biSizeImage = 0;
    bmiHeader.biXPelsPerMeter = 3780;
    bmiHeader.biYPelsPerMeter = 3780;
    bmiHeader.biClrUsed = 0;
    bmiHeader.biClrImportant = 0;

    // Get bitmap data
    int dataSize = screenWidth * screenHeight * 3; // 3 bytes per pixel (24-bit)
    unsigned char* data = new unsigned char[dataSize];
    GetDIBits(hMemoryDC, hBitmap, 0, screenHeight, data, (BITMAPINFO*)&bmiHeader, DIB_RGB_COLORS);

    // Write bitmap data to file
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        // Bitmap file header
        unsigned char bmpHeader[14] = {
            'B', 'M', // Signature
            0, 0, 0, 0, // File size (placeholder)
            0, 0, // Reserved
            0, 0, // Reserved
            54, 0, 0, 0  // Data offset
        };

        // Calculate file size
        unsigned int fileSize = 14 + sizeof(BITMAPINFOHEADER) + dataSize;
        bmpHeader[2] = (unsigned char)(fileSize);
        bmpHeader[3] = (unsigned char)(fileSize >> 8);
        bmpHeader[4] = (unsigned char)(fileSize >> 16);
        bmpHeader[5] = (unsigned char)(fileSize >> 24);

        // Write headers and data
        file.write(reinterpret_cast<char*>(bmpHeader), 14);
        file.write(reinterpret_cast<char*>(&bmiHeader), sizeof(BITMAPINFOHEADER));
        file.write(reinterpret_cast<char*>(data), dataSize);
        file.close();
    }

    // Clean up
    delete[] data;
    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(hwnd, hScreenDC);
    return;
}

int main(){
    char scrnShot[] = "screenshot";
    auto start_time = std::chrono::high_resolution_clock::now();
    int fileNum = 0;
    char fileNumApnd[30];
    while(fileNum < 1){
        //put a name for each individual screenshot and set as bmp
        strcpy(scrnShot, "screenshot");
        itoa(fileNum, fileNumApnd, 10); //add a number to end of screenshot in the 0-10 decimal system
        strcat(scrnShot, fileNumApnd);
        strcat(scrnShot, ".bmp");
        //capture the actual bmp image
        CaptureScreenToBitmap(scrnShot);
        fileNum++;
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;

    std::cout << time/std::chrono::microseconds(1) << std::endl;
return 0;
}

