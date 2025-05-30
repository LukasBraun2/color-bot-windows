#include <iostream>
#include <Windows.h>

int main() {
    POINT p;
    COLORREF color;
    HDC hDC;
    RECT rect;
    int width = 0;
    int height = 0;
    // Get the device context for the screen
    hDC = GetDC(NULL);
    HWND hWndDest = FindWindow(NULL, "Conquest"); //change second parameter to window name for smaller windows
    // Get the current cursor position
    if (GetCursorPos(&p)) {
        //ScreenToClient(hWndDest, &p);
        //p.x and p.y are now relative to hwnd's client area
        if(GetWindowRect(hWndDest, &rect))
        {
            width = rect.right - rect.left-293;
            height = rect.bottom-173 - rect.top;
        }
        std::cout << p.x-width << " " << p.y-height << std::endl;
            
        
        // Retrieve the color at that position
        //color = GetPixel(hDC, p.x, p.y);

        // Extract RGB values
        //int red = GetRValue(color);
        //int green = GetGValue(color);
        //int blue = GetBValue(color);

        // Print the color values
        //std::cout << "RGB: " << red << " " << green << " " << blue << std::endl;
    } else {
        std::cerr << "Error getting cursor position." << std::endl;
        ReleaseDC(GetDesktopWindow(), hDC);
        return 1;
    }

    // Release the device context
    ReleaseDC(GetDesktopWindow(), hDC);

    return 0;
}