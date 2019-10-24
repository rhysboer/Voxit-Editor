// VOXEL FRAG
#version 330 core

uniform sampler2D shadowMap;

in vec2 texCoord;
in vec3 colour;
in vec3 normal;
//in vec3 sun;
in vec3 sunPos;
in vec3 fragPos;

// SETTINGS IN
in float showOutline;
in float showLighting;
in float showShadow;

// LIGHTING IN
in vec4 fragLightSpace;

// Colour Out
out vec4 FragColor;

float Shadow(vec4 lightSpace)
{
    // perform perspective divide
    vec3 projCoords = lightSpace.xyz / lightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	
	//vec3 lightDir = normalize(sunPos - fragPos);
	//float bias = max(0.0005 * (1.0 - dot(normal, lightDir)), 0.001);  
    float shadow = currentDepth - 0.001f > closestDepth  ? 1.0 : 0.0;
	
	if(projCoords.z > 1.0 || dot(normal, sunPos) == 0.0f)
		shadow = 0.0;
	
	return shadow;
}

void main()
{
	// LIGHTING
	vec3 diffuse = vec3(1.0f);
	
	if(showLighting > 0.5f){
		//vec3 direction = normalize(sunPos);
		diffuse = max(dot(normal, sunPos), 0.3f) * vec3(1.0f);
	}
    
	if(showOutline > 0.5f) {
		float radius = 0.98f;
		
		float x = step(texCoord.x, radius);
		float y = step(texCoord.y, radius);
		float dx = step(1.0f - texCoord.x, radius);
		float dy = step(1.0f - texCoord.y, radius);
	
		vec3 fragment;
		float shade = Shadow(fragLightSpace);
		if(shade < 0.9){
			fragment = vec3((diffuse * colour) * (x*dx*y*dy));
		}else{
			fragment = vec3(((1.3 - shade) * vec3(1.0) * colour) * (x*dx*y*dy));
		}
		
		FragColor = vec4(fragment, 1.0f);
	

		//FragColor = vec4(((diffuse * colour.rgb) * (1.3 - Shadow(fragLightSpace))) * (x*dx*y*dy) , 1.0f); // * (x*dx*y*dy)
	}else{
		FragColor = vec4((diffuse * colour.rgb) * (1.0 - Shadow(fragLightSpace)), 1.0f);
	}
} 