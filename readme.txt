Students:                 | NUSP

Patrick Abrahão Menani    | 8941050
Vinícius Pessoa Duarte    | 8941043

The program can be compiled by opening the project file on qtCreator and building it.
The code is in c++.

The program uses a light source on the point vLightPosition (0.0, 4, 5.0). To change this point,
just change the variable inside the method initializeGL().

Our shininess goes from 0 to 64. When using Blinn-Phong and Gouraud shading, the object has specular
color only for small values of shininess.

Inicially, the program has a null image as a texture, making the object all black. By clicking on the
Texture menu, is possible to load a texture. Also, at the program start, the texture is set to standard,
i.e., the texture is just applied to the final color, not changing normals nor vertex position.

Gouraud shading still look strange, but we were not able to fix it.

