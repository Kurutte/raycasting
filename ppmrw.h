// protect from multiple compiling
#ifndef PPMRW_H
#define PPMRW_H

typedef struct Pixel
{
   unsigned char red, green, blue;
} Pixel;

typedef struct Image
{
   unsigned int width, height;
   Pixel *pixel;
} Image;

int mainTest( int argc, char *argv[] );

Pixel *readFile( FILE *inFile, char *buffer, char *comment, unsigned int *width,
unsigned int *height, unsigned int *colors, char* fromFile, Pixel *pixel, int *numComments );

void writeFile( FILE *outFile, char *buffer, char *comment, unsigned int width,
unsigned int height, unsigned int colors, char* toFile, Pixel *pixel, int numComments );

#endif