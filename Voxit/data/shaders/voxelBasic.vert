// VOXEL VERT
#version 330 core

layout (location = 0) in vec3 aVer; // VERTICE POSITION
layout (location = 1) in vec2 aTex; // TEXTURE COORDINATES
layout (location = 2) in vec3 aNom; // NORMALS
layout (location = 3) in vec3 aCol; // COLOUR

uniform vec3 _pos;
uniform mat4 _lightMatrix;

void main()
{

	mat4 model = mat4(1.0f);
	model[3] = vec4(_pos, 1.0f);
	
	gl_Position =  _lightMatrix * model * vec4(aVer, 1.0f);
}