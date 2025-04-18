#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <winuser.h>
#include <unistd.h>
#pragma pack(2)


typedef struct
{
    char signature[2];
    unsigned int fileSize;
    unsigned int reserved;
    unsigned int offset;
} BmpHeader;

typedef struct
{
    unsigned int headerSize;
    unsigned int width;
    unsigned int height;
    unsigned short planeCount;
    unsigned short bitDepth;
    unsigned int compression;
    unsigned int compressedImageSize;
    unsigned int horizontalResolution;
    unsigned int verticalResolution;
    unsigned int numColors;
    unsigned int importantColors;

} BmpImageInfo;

typedef struct
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    //unsigned char reserved; Removed for convenience in fread; info.bitDepth/8 doesn't seem to work for some reason
} Rgb;


void moveMouseRelative(HWND, int, int);
void SendMouseClick(HWND, int, int);

int main( int argc, char **argv ) {

    FILE *inFile;
    BmpHeader header;
    BmpImageInfo info;
    Rgb *palette;
    int i = 0;
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);
    FILE *outFile;




  // printf( "Opening file %s for writing.\n", "PixelData.txt" );
//    FILE *outFile = fopen( "PixelData.txt", "wb" );
/*    if( !outFile ) {
        printf( "Error opening outputfile.\n" );
        return -1;
    }*/

    Rgb *pixel = (Rgb*) malloc( sizeof(Rgb) );
    int read, j;
    while(1){
        HWND hWnd = FindWindow(NULL, "Conquest");

        if (!hWnd) {
            printf("Window not found!\n");
            return 1;
        }

        RECT rect;
        int width = 0;
        int height = 0;
        if(GetWindowRect(hWnd, &rect))
        {
          width = rect.right - rect.left;
          height = rect.bottom - rect.top;
        }
        int found = 0;

   //     system("capImg.exe");

            //    printf( "Opening file %s for reading.\n", "screenshot0.bmp" );
        inFile = fopen( "screenshot0.bmp", "rb" );
        if( !inFile ) {
            printf( "Error opening file here %s.\n", argv[1] );
            return -1;
        }

        if( fread(&header, 1, sizeof(BmpHeader), inFile) != sizeof(BmpHeader) ) {
            printf( "Error reading bmp header.\n" );
            return -1;
        }

        if( fread(&info, 1, sizeof(BmpImageInfo), inFile) != sizeof(BmpImageInfo) ) {
            printf( "Error reading image info.\n" );
            return -1;
        }


    //    SetPixel(150, 153);
    //    printf( "Width x Height: %i x %i\n", info.width, info.height );
        if( info.numColors > 0 ) {
    //        printf( "Reading palette.\n" );
            palette = (Rgb*)malloc(sizeof(Rgb) * info.numColors);
            if( fread(palette, sizeof(Rgb), info.numColors, inFile) != (info.numColors * sizeof(Rgb)) ) {
                printf( "Error reading palette.\n" );
                return -1; // error
            }
        }
        outFile = fopen("rgbVals.txt", "w");
        fprintf(outFile, "%d, %d\n", height, width);
        int rowBytes = 0;
        int padding = 0;
        for( j=0; j<height; j++ ) { //may have to change once screenshots are figured out
            //printf( "------ Row %d\n", j+1 );
            rowBytes = width * sizeof(Rgb);
            padding = (4-(rowBytes%4))%4;
            read = 0;
            fseek(inFile, padding, SEEK_CUR);
            for( i=0; i<width; i++ ) { //may have to change once screenshots are figured out
                if( fread(pixel, 1, sizeof(Rgb), inFile) != sizeof(Rgb) ) {
                    printf( "Error reading pixel!\n" );
                }
                fprintf(outFile, "%u,%u,%u\n", pixel->red, pixel->green, pixel->blue);
                read += sizeof(Rgb);
  //              if(i == 277+8 && j == 104+31){  //j is x, i is y i=267-8;
                                                //j=252-31;
      //              printf( "Pixel %d %d: %3d %3d %3d\n", i, j, pixel->red, pixel->green, pixel->blue );
        //        }

                //find pixel of a certain color and click on it.
           /*     if(pixel->red == 54 && pixel->green == 12 && pixel->blue == 58){
                    if(found == 0){
                        found = 1;
                        printf( "Pixel x: %d: y: %d\n", i, j);
                        SetActiveWindow(hWnd);
                        moveMouseRelative(hWnd, i, j);
                        POINT p;
                        GetCursorPos(&p);
                        ScreenToClient(hWnd, &p); 
                        printf("x: %d y: %d\n", p.x, p.y);
                        sleep(1);
                        if(p.x == i && p.y == j){
                            //p.x and p.y are now relative to hwnd's client area
                            printf("x: %d y: %d", p.x, p.y);
                            SendMouseClick(hWnd, i, j);
                            sleep(5);
                        }
                    }
                }*/
            }  
            /*
             if( read % 4 != 0 ) {
                read = 4 - (read%4);
               // printf( "Padding: %d bytes\n", read );
                fread( pixel, read, 1, inFile );
            }*/
        }
    }
    QueryPerformanceCounter(&end);

    double elapsed_time = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart * 1000000.0;

    printf("Elapsed time: %.2f microseconds\n", elapsed_time);

    printf( "Done.\n" );
    fclose(inFile);
    fclose(outFile);
   // fclose(outFile);

    printf( "\nBMP-Info:\n" );
    printf( "Width x Height: %i x %i\n", info.width, info.height );
    printf( "Depth: %i\n", (int)info.bitDepth );

    return 0;

}


void moveMouseRelative(HWND hwnd, int x, int y) {
  // Get the window's position on the screen
  RECT windowRect;
  GetWindowRect(hwnd, &windowRect);

  // Calculate the new mouse position relative to the window
  int newX = windowRect.left + x + 8; //add 8 because window bar is probably included
  int newY = windowRect.top + y + 31; //add 32 because window bar is probably included
//  std::cout << newX << " " << newY << std::flush;
  // Move the mouse cursor
  SetCursorPos(newX, newY);
}

void SendMouseClick(HWND hWnd, int x, int y) {
    POINT p;
    p.x = x;
    p.y = y;

    // Convert screen coordinates to client coordinates
    ScreenToClient(hWnd, &p);


    RECT rect;
    int width = 0;
    int height = 0;
    if(GetWindowRect(hWnd, &rect))
    {
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
    } else {
        printf("error");
    }

    INPUT inputs[2] = {};
    
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    inputs[0].mi.dx = p.x*(65535/GetSystemMetrics(SM_CXSCREEN));
    inputs[0].mi.dy = p.y*(65535/GetSystemMetrics(SM_CYSCREEN));
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    inputs[1].mi.dx = p.x*(65535/GetSystemMetrics(SM_CXSCREEN));
    inputs[1].mi.dy = p.y*(65535/GetSystemMetrics(SM_CYSCREEN));
       
    /*RECT rect = {0};
    GetWindowRect(hWnd, &rect);

    SetForegroundWindow(hWnd);
    SetActiveWindow(hWnd);
    SetFocus(hWnd);
    Sleep(300);*/

    SendInput(3, inputs, sizeof(INPUT));
}
