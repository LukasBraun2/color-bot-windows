set PATH=%PATH%;C:\MinGW\bin\

g++ -o capImg capImg.cpp -static-libgcc -static-libstdc++ -lgdi32

Pause(10);