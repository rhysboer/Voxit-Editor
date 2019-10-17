// VOXEL FRAG
#version 330 core

in vec2 texCoord;
in vec3 colour;
in vec3 normal;
in vec3 sun;

// SETTINGS IN
in float showOutline;
in float showLighting;
in float showShadow;

out vec4 FragColor;

float radius = 0.98f;

void main()
{
	// LIGHTING
	vec3 diffuse = vec3(1.0f);
	
	if(showLighting > 0.5f)
		diffuse = max(dot(normal, -sun), 0.5f) * vec3(1.0f);
    
	if(showOutline > 0.5f) {
		float x = step(texCoord.x, radius);
		float y = step(texCoord.y, radius);
		float dx = step(1.0f - texCoord.x, radius);
		float dy = step(1.0f - texCoord.y, radius);
	
		FragColor = vec4(diffuse * colour.rgb * (x*dx*y*dy), 1.0f);
	}else{
		FragColor = vec4(diffuse * colour.rgb, 1.0f);
	}
	
	//float x = step(texCoord.x, radius);
	//float y = step(texCoord.y, radius);
	//float dx = step(1.0f - texCoord.x, radius);
	//float dy = step(1.0f - texCoord.y, radius);
	//
	//FragColor = vec4(colour * (x*dx*y*dy), 1.0f);
} 