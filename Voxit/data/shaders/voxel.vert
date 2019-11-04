// VOXEL VERT
#version 330 core

layout (location = 0) in vec3 aVer; // VERTICE POSITION
layout (location = 1) in vec2 aTex; // TEXTURE COORDINATES
layout (location = 2) in vec3 aNom; // NORMALS
layout (location = 3) in vec3 aCol; // COLOUR

const int CASCADE_NUM = 3; // Total amount of cascades

struct Light{
	float cascadeSplits[CASCADE_NUM]; 	// Splits between cascades
	vec4 lightDirection; 				// Global Light | XYZ = Direction, W = 1 Enabled / 0 Disabled
	mat4 lightMatrix[CASCADE_NUM]; 		// light Matrices
};

// LIGHT DATA
uniform Light _lightData;

// SETTINGS - 0=Off/1=On
uniform float _showOutline;

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
out vec4 fragLightSpace[CASCADE_NUM];	
out float cascadeSize[CASCADE_NUM]; 
out vec4 sunDirection;
out float clipSpaceZ;

// SETTINGS OUT
out float showOutline;


void main()
{
	// Model Matrix
	mat4 model = mat4(1.0f);
	model[3] = vec4(_pos, 1.0f);
	
	// Out Object Data
	fragPos 	= vec3(model * vec4(aVer, 1.0f));
	colour  	= aCol;
	texCoord 	= aTex;
	normal 		= aNom;
	
	// Out Other Data
	sunDirection = _lightData.lightDirection;
	showOutline = _showOutline;

	fragLightSpace[0] = _lightData.lightMatrix[0] * vec4(fragPos, 1.0f);
	fragLightSpace[1] = _lightData.lightMatrix[1] * vec4(fragPos, 1.0f);
	fragLightSpace[2] = _lightData.lightMatrix[2] * vec4(fragPos, 1.0f);
	
	cascadeSize[0] = _lightData.cascadeSplits[0];
	cascadeSize[1] = _lightData.cascadeSplits[1];
	cascadeSize[2] = _lightData.cascadeSplits[2];
	
	// Vertice Position
	gl_Position =  _projection * _view * model * vec4(aVer, 1.0f);
	
	clipSpaceZ = gl_Position.z;
}