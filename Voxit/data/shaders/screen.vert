// VOXEL VERT
#version 330 core

layout (location = 0) in vec3 aVer; // VERTICE POSITION
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTex; // TEXTURE POSITION

uniform float depthIndex;
uniform vec3 position;

out float index;
out vec2 texCoords;

void main()
{
	index = depthIndex;
	texCoords = aTex;
	gl_Position = vec4(aVer + position, 1.0f);
}