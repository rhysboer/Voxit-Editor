// VOXEL VERT
#version 330 core

layout (location = 0) in vec3 aVer; // VERTICE POSITION
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTex; // TEXTURE POSITION

out vec2 texCoords;

void main()
{
	texCoords = aTex;
	gl_Position = vec4(aVer, 1.0f);
}