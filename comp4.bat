set PATH=%PATH%;C:\MinGW\bin\

g++ -o setPixel setPixel.cpp -static-libgcc -lgdi32 -luser32

Pause(10);