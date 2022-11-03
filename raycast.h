// protect from multiple compiling
#ifndef RAYCAST_H
#define RAYCAST_H
// Create structure for the camera
typedef struct Camera
{
	// position of camera
	float position[3];

	// NOTE: camera faces negative-z direction

	// width of camera
	float width;

	// height of camera
	float height;
} Camera;

// Create sceneObjects
typedef struct SceneObject
{
	// used to see whether sphere or plane is used
		// 0 = sphere, 1 = plane
	int type;

	// color of object (both)
	float color[3];

	// position of object 
	float position[3];

	// (For spheres) radius of the sphere
	float radius;

	// (For planes) normal vector of the plane
	float nVector[3];

} SceneObject;

int shoot(float* ray, SceneObject *sceneObjects, float *origin, int loopIndex);
//float* shade(float* hitPos);

#define BYTE_SIZE 256

#endif