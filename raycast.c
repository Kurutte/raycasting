#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "raycast.h"
#include "v3math.h"
#include "ppmrw.h"


int main( int argc, char* argv[] )
{
	// Check for correct number of arguments
	if ( argc != 5 )
	{
		// report error not proper program call
		fprintf(stderr, "\nError: Incorrect amount of arguments supplied. Please use:");
		fprintf(stderr, "raycast width height input.json output.ppm");
		printf("\nProgram End\n\n");
		return 1;
	}

	// Check if input file is .scene
	if (strstr(argv[3], ".scene") == NULL)
	{
		// report error not proper program call
		fprintf(stderr, "\nError: Incorrect input file type. Please use a \" .scene \" file.");
		printf("\nProgram End\n\n");
		return 1;
	}

	// Open input and output files
	   // fopen()
	FILE *outFile = fopen(argv[4], "w");
	FILE *inFile = fopen(argv[3], "r");
	char buffer[ BYTE_SIZE ];
	float screenWidth, screenHeight;
	float pixHeight, pixWidth;
	screenWidth = atof(argv[1]);
	screenHeight = atof(argv[2]);
	SceneObject sceneObjects[128];
	int sceneIndex = 0;
	Image *outImage = (Image *)malloc(sizeof(Image));
	Camera *camera = (Camera *)malloc(sizeof(Camera));
	camera->position[0] = 0;
	camera->position[1] = 0;
	camera->position[2] = 0;

	// load in camera
	fgets( buffer, BYTE_SIZE, inFile);
	sscanf( buffer, "camera, width: %f, height: %f", &camera->width, &camera->height );

	// load in rest of the scene file
	while(fgets( buffer, BYTE_SIZE, inFile) != NULL)
	{
		if(sscanf( buffer, "sphere, color: [%f, %f, %f], position: [%f, %f, %f], radius: %f", 
			&sceneObjects[sceneIndex].color[0], &sceneObjects[sceneIndex].color[1],
			&sceneObjects[sceneIndex].color[2], &sceneObjects[sceneIndex].position[0],
			&sceneObjects[sceneIndex].position[1], &sceneObjects[sceneIndex].position[2],
			&sceneObjects[sceneIndex].radius ) == 7)
		{
			sceneObjects[sceneIndex].type = 0;
		}
		else if(sscanf( buffer, "plane, color: [%f, %f, %f], position: [%f, %f, %f], normal: [%f, %f, %f]", 
			&sceneObjects[sceneIndex].color[0], &sceneObjects[sceneIndex].color[1],
			&sceneObjects[sceneIndex].color[2], &sceneObjects[sceneIndex].position[0],
			&sceneObjects[sceneIndex].position[1], &sceneObjects[sceneIndex].position[2],
			&sceneObjects[sceneIndex].nVector[0], &sceneObjects[sceneIndex].nVector[1], &sceneObjects[sceneIndex].nVector[2] ) == 9)
		{
			sceneObjects[sceneIndex].type = 1;
		}
		sceneIndex++;
	}

	// Initialize variables
	   // coord for pixels in pixmap.
	float pX;
	float pY;
	float pZ = camera->position[2] - 1; 	      // Viewplane is always at (0,0,-1)
	   // ray that gets shot
	float *ray = malloc(sizeof(float) * 3); 
	   // index of closest object that is hit
	int hitIndex;

	// Get Height/Width of pixel density for the viewplane
	pixHeight = camera->height / screenHeight;
	pixWidth = camera->width / screenWidth;

	// Set image height/width to what was specified
	outImage->width = screenWidth;
	outImage->height = screenHeight;

	// Initialize pixel array variables 
	outImage->pixel = (Pixel *)malloc(sizeof(Pixel) * (outImage->width) * (outImage->height) );
	int pixelPos;

	// Loop through the width of the camera
	for (int i = 0; i < screenHeight; i++)
	{
		// Get the y-cord for the pixel to cast a ray
		pY = camera->position[1] - camera->height / 2 + pixHeight * (i + 0.5);

		// Loop through the height of the camera
		for (int j = 0; j < screenWidth; j++)
		{
			// Get the x-cord for the pixel to cast a ray
			pX = camera->position[0] - camera->width / 2 + pixWidth * (j + 0.5);

 			// Make the unit ray vector
			   // Make array for position of pixel
			float pCoords[3] = {pX, pY, pZ};
			   // Create ray from coordinates of pixel
			v3_from_points(ray, camera->position, pCoords);
			   // Normalize Ray
			v3_normalize(ray, ray);

			// Get the index of the "first object hit"
			hitIndex = shoot(ray, sceneObjects, pCoords, sceneIndex);
		    pixelPos = i*screenWidth + j + 1;

			// Check if nothing was hit (-1)
			if (hitIndex == -1)
			{
				// Shade pixel black (default color)
				outImage->pixel[pixelPos].red = 0 ;
				outImage->pixel[pixelPos].green = 0;
				outImage->pixel[pixelPos].blue = 0;
			}
			else
			{
				// Shade pixel color of closest object hit
				outImage->pixel[pixelPos].red = sceneObjects[hitIndex].color[0]*255;
				outImage->pixel[pixelPos].green = sceneObjects[hitIndex].color[1]*255;
				outImage->pixel[pixelPos].blue = sceneObjects[hitIndex].color[2]*255;
			}			
		}
	}

	// Write pixel colors to ppm file
	char* test1= "test";
    writeFile( outFile, test1, test1, outImage->width, outImage->height, 255, "3", outImage->pixel, 1 );
}

int shoot(float* ray, SceneObject *sceneObjects, float* origin, int loopIndex)
{
	// Initialize variables
	   // General variables
	int sceneIndex = 0;
	int hitIndex = -1;
	float t;
	float closestT = 1000;
	   // For plane calcs
	float d;
	float planeDenom;
	float planeNum;
	   // For sphere calcs
	float a;
	float b;
	float c;
	float discriminant;

	// Loop through all scene objects
	while (sceneIndex < loopIndex)
	{
		// Check if object is a sphere
		if (sceneObjects[sceneIndex].type == 0)
		{
			// Put ray + sphere equation into quadratic formula
			   // Find A of quadratif formula
			a = ray[0] * ray[0] + ray[1] * ray[1] + ray[2] * ray[2];
			
			   // Find B of quadratic formula
			b = 2 * (ray[0] * (origin[0] - sceneObjects[sceneIndex].position[0]) +
				ray[1] * (origin[1] - sceneObjects[sceneIndex].position[1]) +
				ray[2] * (origin[2] - sceneObjects[sceneIndex].position[2]));

			   // Find C of quadratic formula
			c = (origin[0] - sceneObjects[sceneIndex].position[0]) * (origin[0] - sceneObjects[sceneIndex].position[0]) +
				(origin[1] - sceneObjects[sceneIndex].position[1]) * (origin[1] - sceneObjects[sceneIndex].position[1]) +
				(origin[2] - sceneObjects[sceneIndex].position[2]) * (origin[2] - sceneObjects[sceneIndex].position[2]) - 
				sceneObjects[sceneIndex].radius * sceneObjects[sceneIndex].radius;

			// Calculate discriminant (b^2 - 4c)
			discriminant = b * b - 4 * a * c;

			// Check if ray hits sphere (discriminant will be greater than 0)
			if (discriminant > 0)
			{
				// Calculate with (-) of quadratic first
				t = (-b - sqrt(discriminant)) * (1 / (2*a));

				// Check if t is not positive (value we dont want)
				if (t < 0)
				{
					// Calculate with (+) of quadratic)
					t = (-b + sqrt(discriminant)) * (1 / (2*a));
				}

				// Check if t is new lowest t
				if (t < closestT)
				{
					// Assign new closest T
					closestT = t;

					// Assign new hitIndex
					hitIndex = sceneIndex;
				}
			}

		}

		// Check if object is a plane
		else if (sceneObjects[sceneIndex].type == 1)
		{
			// Calculate dot product of plane normal + ray (denominator)
			planeDenom = v3_dot_product(sceneObjects[sceneIndex].nVector, ray);

			// Check if plane and ray are not parallel
			if (planeDenom != 0.0)
			{
				// Calculate dot product of plane normal & Distance from origin of camera to point on plane (numerator)
				   // Find what scalar D is
				d = v3_dot_product(sceneObjects[sceneIndex].nVector, sceneObjects[sceneIndex].position);
				   // Make d negative
				d = d * -1;
				   // Get dot product of normal and origin
				planeNum = v3_dot_product(sceneObjects[sceneIndex].nVector, origin);
				   // Add d to result
				planeNum = planeNum + d;
				   // Make planeNum negative
				planeNum = planeNum * -1;

				// Calculate scalar t for hit
				t = planeNum / planeDenom;

				// Check if t is not behind the camera (a negative value) and is closest t (lower than closestT)
				if (t > 0.0 && t < closestT)
				{
					// Assign new closestT
					closestT = t;

					// Assign new hitIndex
					hitIndex = sceneIndex;
				}
			}
		}
		// move to next object in list
		sceneIndex++;
	}

	// Return hit index (200 = no-hit)
	return hitIndex;
}

/*// NOT USED FOR NOW
float *illuminate(float *hitPos)
{
	// Check if ray hit nothing (NO_HIT)
		// Return default color (black)
	
	// Otherwise, loop through all objects
	return 0;
}*/