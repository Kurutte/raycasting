#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppmrw.h"

#define BYTE_SIZE 256

int mainTest( int argc, char *argv[] )
{
	// Check for correct number of args
	if( argc != 4 )
	{
		// report error not proper program call
		fprintf( stderr, "\nError: Incorrect amount of arguments supplied. Please use:" );
		fprintf( stderr, "ppmrw # input.ppm output.ppm" );
		printf( "\nProgram End\n\n" );
		return 1;
	}

	// open input/output file
	FILE *outFile = fopen( argv[ 3 ], "wb" );
	FILE *inFile = fopen( argv[ 2 ], "rb" );


	// check whether converting to P3 or P6
	if( strncmp( argv[ 1 ], "3", 2) != 0 && strncmp( argv[ 1 ], "6", 2 ) != 0 )
	{
		fprintf( stderr, "\nError: Program only supports P6 and P3 file conversion." );
		printf( "\nProgram End\n\n" );
		return 1;
	}


	printf( "\nProgram Start" );
	printf( "\n=============\n");

	// initialize variables
	char buffer[ BYTE_SIZE ];
	char *comment;
	comment = (char *)malloc(sizeof(char));
	char *magicNumber = (char *)malloc(sizeof(char));
	Image *image;
	image = (Image *)malloc(sizeof(Image));
	unsigned int colors;
	int numComments = 0;


   image->pixel = readFile( inFile, buffer, comment, &image->width, &image->height, &colors,
   	magicNumber, image->pixel, &numComments );
   
	writeFile( outFile, buffer, comment, image->width, image->height, colors, argv[1], image->pixel, numComments );
	

	// close output file
	fclose( outFile );
	// close input file
	fclose( inFile );
	free(image);
	free(magicNumber);
	free(comment);

	printf( "\nProgram End\n\n" );
	return 0;
}

Pixel *readFile( FILE *inFile, char *buffer, char *comment, unsigned int *width,
unsigned int *height, unsigned int *colors, char* fromFile, Pixel *pixel, int *numComments )
{
	// read magic number
	fgets( buffer, BYTE_SIZE, inFile);
	//save magic number
	sscanf( buffer, "%s", fromFile );
	// read comment
   //fgets( buffer, BYTE_SIZE, inFile );

   while( strncmp( fgets( buffer, BYTE_SIZE, inFile ), "#", 1) == 0 )
   {
   	(*numComments)++;
   }

   // save comment
   //sscanf( buffer, "%s", comment );
   // read width
	//fscanf( inFile, "%u", width );
	// read height
	//fscanf( inFile, "%u", height );
	sscanf( buffer, "%u %u", width, height );
	// read max color value
	fscanf( inFile, "%u", colors );

	pixel = (Pixel *)malloc(sizeof(Pixel) * (*width) * (*height) );

	if( strncmp( fromFile, "P6", 2) == 0 )
	{			
			unsigned char temp;

			fread( pixel, sizeof( Pixel ), (*width) * (*height), inFile );
			for( int index = 0; index < (*width) * (*height); index++ )
			{
				temp = pixel[index].red;
				pixel[index].red = pixel[index].green;
				pixel[index].green = pixel[index].blue;
				pixel[index].blue = temp;
			}

	}
	else if( strncmp( fromFile, "P3", 2) == 0 )
	{
		for( int index = 0; index < (*width) * (*height); index++ )
		{
		   int asciiVal;
		   fscanf( inFile, "%d", &asciiVal );
      	   pixel[ index ].red = asciiVal;
      	   fscanf( inFile, "%d", &asciiVal );
      	   pixel[ index ].green = asciiVal;
      	   fscanf( inFile, "%d", &asciiVal );
      	   pixel[ index ].blue = asciiVal;
		}
	}

   return pixel;

}

void writeFile( FILE *outFile, char *buffer, char *comment, unsigned int width,
unsigned int height, unsigned int colors, char* toFile, Pixel *pixel, int numComments )
{
	// write magic number
	fprintf( outFile, "P%s\n", toFile );

	// write comments to output file
	for( int i = 0; i < numComments; i++)
	{
		fprintf( outFile, "#\n" );
	}
	
	// write width to output file
	fprintf( outFile, "%u ", width );
	
	//write height to output file
	fprintf( outFile, "%u", height );
	
	// write max color value to output file
	fprintf( outFile, "\n%u\n", colors );

	if( strncmp( toFile, "3", 2) == 0 )
	{
		for( int index = 0; index < width * height; index++ )
		{
		fprintf( outFile, " %u %u %u ", 
			pixel[ index ].red, pixel [ index ].green, pixel[ index ].blue );

		}
	}
	else if( strncmp( toFile, "6", 2) == 0 )
	{
		fwrite( pixel, 3 * width, height, outFile );
	}
	

}

