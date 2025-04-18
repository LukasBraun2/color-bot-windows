set PATH=%PATH%;C:\MinGW\bin\

g++ -o mouseRGB mouseRGB.cpp -static-libgcc -static-libstdc++ -lgdi32 -luser32

Pause(10);