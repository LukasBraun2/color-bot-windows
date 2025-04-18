set PATH=%PATH%;C:\MinGW\bin\

gcc -o readPix readPix.c -static-libgcc -lgdi32 -luser32

Pause(10);