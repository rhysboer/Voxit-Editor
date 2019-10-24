// VOXEL VERT
#version 330 core

layout (location = 0) in vec3 aVer; // VERTICE POSITION
layout (location = 1) in vec2 aTex; // TEXTURE COORDINATES
layout (location = 2) in vec3 aNom; // NORMALS
layout (location = 3) in vec3 aCol; // COLOUR

// SETTINGS - 0=Off/1=On
uniform float _showOutline;
uniform float _showLighting; 	
//uniform float _showShadow;	
//uniform vec3 _lightDire;	// LIGHT DIRECTION
uniform vec3 _sunPosition;

// LIGHTING
uniform mat4 _lightMatrix;

// OBJECT DATA - MVP
uniform vec3 _pos;          // POSITION
uniform mat4 _view;			// WORLD
uniform mat4 _projection;	// CAMERA

// DATA OUT
out vec2 texCoord;
out vec3 colour;
out vec3 normal;
out vec3 fragPos;

// LIGHTING OUT
out vec4 fragLightSpace;
out vec3 sunPos;

// SETTINGS OUT
out float showOutline;
out float showLighting;
out float showShadow;
//out vec3 sun;

void main()
{
	mat4 model = mat4(1.0f);
	model[3] = vec4(_pos, 1.0f);
		
	gl_Position =  _projection * _view * model * vec4(aVer, 1.0f);
	
	fragPos 	= vec3(model * vec4(aVer, 1.0f));
	colour  	= aCol;
	texCoord 	= aTex;
	normal 		= aNom;
	//sun 		= _lightDire;
	sunPos 		= _sunPosition;
	fragLightSpace = _lightMatrix * vec4(fragPos, 1.0f);
	
	
	// SETTINGS
	showOutline 	= _showOutline;
	showLighting 	= _showLighting;
	//showShadow		= _showShadow;
}