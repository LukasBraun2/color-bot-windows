// Open the BMP file for reading and writing
inFile = fopen("screenshot0.bmp", "rb+");
if (!inFile) {
    printf("Error opening file.\n");
    return -1;
}

// Read headers
fread(&header, 1, sizeof(BmpHeader), inFile);
fread(&info, 1, sizeof(BmpImageInfo), inFile);

// Pixel coordinates (x = col, y = row from bottom)
int targetX = 153;
int targetY = 150;

// Calculate row padding (each row must be a multiple of 4 bytes)
int rowSize = ((info.bitDepth * info.width + 31) / 32) * 4;

// BMP stores pixels from bottom to top
int pixelOffset = header.offset + (info.height - 1 - targetY) * rowSize + targetX * 3;

// Seek to the pixel's offset
fseek(inFile, pixelOffset, SEEK_SET);

// Write new RGB values (red in this case)
Rgb newPixel = { .blue = 0, .green = 255, .red = 0 };
fwrite(&newPixel, sizeof(Rgb), 1, inFile);

fclose(inFile);
printf("Pixel (%d, %d) changed to red.\n", targetX, targetY);
