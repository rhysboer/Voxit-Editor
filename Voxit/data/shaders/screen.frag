// VOXEL FRAG
#version 330 core

out vec4 FragColor;

in float index;
in vec2 texCoords;

uniform sampler2D depthMap_zro;
uniform sampler2D depthMap_one;
uniform sampler2D depthMap_two;

void main() 
{ 
	float depth = 1;
	if(floor(index) == 0.0f) { depth = texture(depthMap_zro, vec3(texCoords, 0).xy).r; }
	if(floor(index) == 1.0f) { depth = texture(depthMap_one, vec3(texCoords, 0).xy).r; }
	if(floor(index) == 2.0f) { depth = texture(depthMap_two, vec3(texCoords, 0).xy).r; }
	
	/*
	depth = texture(depthMap[0], vec3(texCoords, 0).xy).r;
	depth = texture(depthMap[1], vec3(texCoords, 0).xy).r;
	depth = texture(depthMap[2], vec3(texCoords, 0).xy).r;
	*/
	
	FragColor = vec4(vec3(depth), 1.0f); 

	// P = vec3(U, V, array pos);
	// gvec4 texture(	gsampler2DArray sampler, vec3 P, [float bias]);
} 