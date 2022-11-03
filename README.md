# Authors

Stefan Mihailovic
Eli Barela

# Usage

The main purpose for this program is to read scene files, raycast plane and sphere primitives, and write to P3 ppm output.

Scene files contain objects for the scene to be output to the image. The input.scene file can contain lines akin to: 

- camera, width: 2.0, height: 2.0
- sphere, color: [1, 0, 0], position: [0, 1, -5], radius: 2
- plane, color: [0, 1, 0], position: [0, -2, 0], normal: [0, 1, 0]

Colors are RGB values from 0-1, positions/normal vectors are stored as arrays of 3 floats.

Command Line: ``` raycast width height input.scene output.ppm ``` 	
