// VOXEL FRAG
#version 330 core
out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D depthMap;

void main() 
{ 
	float depth = texture(depthMap, texCoords).r;
	FragColor = vec4(vec3(depth), 1.0f); 
} 