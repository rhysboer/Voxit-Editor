// VOXEL VERT
#version 330 core

layout (location = 0) in vec3 aVer; // VERTICE POSITION
layout (location = 1) in vec2 aTex; // TEXTURE COORDINATES
layout (location = 2) in vec3 aNom; // NORMALS
layout (location = 3) in vec3 aCol; // COLOUR

//layout (location = 4) in vec3 aPos; // MODEL POSITION

// SETTINGS - 0=Off/1=On
uniform float _showOutline;
uniform float _showLighting; 	
uniform float _showShadow;	
uniform vec3 _lightDire;	// LIGHT DIRECTION

// OBJECT / CAMERA DATA
uniform vec3 _pos;          // (TEST) Position!!
uniform mat4 _view;			// WORLD
uniform mat4 _projection;	// CAMERA

out vec2 texCoord;
out vec3 colour;
out vec3 normal;

// SETTINGS OUT
out float showOutline;
out float showLighting;
out float showShadow;
out vec3 sun;

void main()
{
	mat4 model = mat4(1.0f);
	//model[3] = vec4(aPos, 1.0f);
	model[3] = vec4(_pos, 1.0f);
		
	gl_Position =  _projection * _view * model * vec4(aVer, 1.0f);
	
	colour  	= aCol;
	texCoord 	= aTex;
	normal 		= aNom;
	sun 		= _lightDire;
	
	// SETTINGS
	showOutline 	= _showOutline;
	showLighting 	= _showLighting;
	showShadow		= _showShadow;
}