// VOXEL FRAG
#version 330 core
#define RADIUS 0.98f // Outline Radius

const int CASCADE_NUM = 3;

// Shadow Maps
uniform sampler2D shadowMap[CASCADE_NUM];

// Object Data
in vec2 texCoord;
in vec3 colour;
in vec3 normal;
in vec3 fragPos;

// SETTINGS IN
in float showOutline;

// LIGHTING IN
in vec4 fragLightSpace[CASCADE_NUM];
in float cascadeSize[CASCADE_NUM]; 
in vec4 sunDirection;
in float clipSpaceZ;

// Colour Out
out vec4 FragColor;

float Shadow(vec4 lightSpace, int shadowIndex)
{
    vec3 projCoords = lightSpace.xyz * 0.5 + 0.5;

    float closestDepth = texture(shadowMap[shadowIndex], projCoords.xy).r; 
	
    float currentDepth = projCoords.z;

    float shadow = currentDepth - 0.0008f > closestDepth  ? 1.0 : 0.0; // - 0.0008f
	
	if(projCoords.z > 1.0)
		shadow = 0.0;
	
	return shadow;
}

void main()
{
	// OUTLINE
	float x = step(texCoord.x, RADIUS);
	float y = step(texCoord.y, RADIUS);
	float dx = step(1.0f - texCoord.x, RADIUS);
	float dy = step(1.0f - texCoord.y, RADIUS);
	
	float outline = (round(showOutline) >= 1.0) ? (x*dx*y*dy) : 1.0f;
	
	vec3 fragment = colour;
	
	// LIGHTING
	if(round(sunDirection.w) >= 1.0f){ 
		// DIFFUSE
		float diffuse = max(dot(normal, sunDirection.xyz), 0.3f);
		
		// SHADOWING
		float shade = 0.0f;
		for(int i = 0; i < CASCADE_NUM; i++){
			if(clipSpaceZ <= cascadeSize[i]){// cascadeSize[i]
				//if(i == 0) fragment = vec3(1.0f, 0.0f, 0.0f);
				//if(i == 1) fragment = vec3(0.0f, 1.0f, 0.0f);
				//if(i == 2) fragment = vec3(0.0f, 0.0f, 1.0f);
				shade = Shadow(fragLightSpace[i], i);
				break;
			}
		}

		fragment = (shade > 0.5f) ? colour * (1.3 - shade) : colour * diffuse;
	}
	
	//fragment = vec3(clipSpaceZ, clipSpaceZ, clipSpaceZ);
	
	// END
	FragColor = vec4(fragment * outline, 1.0f);
} 



/*




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
		
		//FragColor = vec4(fragment, 1.0f);
	
		FragColor = vec4((colour.rgb * 1.3 - shade) * (x*dx*y*dy), 1.0f);
	

		//FragColor = vec4(((diffuse * colour.rgb) * (1.3 - Shadow(fragLightSpace))) * (x*dx*y*dy) , 1.0f); // * (x*dx*y*dy)
	}else{
		FragColor = vec4((diffuse * colour.rgb) * (1.0 - Shadow(fragLightSpace)), 1.0f);
	}





*/