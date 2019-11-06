// VOXEL FRAG
#version 330 core

const int CASCADE_NUM = 4;
const float BIAS = 0.0007f;
const float RADIUS = 0.98f;

// Shadow Maps
uniform sampler2DShadow shadowMap[CASCADE_NUM];

// Object Data
in vec2 texCoord;
in vec3 colour;
in vec3 normal;
in vec3 fragPos;

// SETTINGS IN
in float showOutline;

// LIGHTING IN
in vec4 fragLightSpace[CASCADE_NUM];
in vec4 sunDirection;
in float cascadeSize[CASCADE_NUM]; 
in float clipSpaceZ;

// Colour Out
out vec4 FragColor;

float ShadowCalc(vec4 lightSpace, int shadowIndex)
{
    vec3 projCoords = lightSpace.xyz * 0.5 + 0.5;
	
	float closestDepth = texture(shadowMap[shadowIndex], vec3(projCoords.xy, projCoords.z - BIAS));

    float shadow = projCoords.z - BIAS > closestDepth  ? 1.0 : 0.0;
	
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
			if(clipSpaceZ <= cascadeSize[i]){
				shade = ShadowCalc(fragLightSpace[i], i);
				break;
			}
		}

		fragment = (shade > 0.5f) ? colour * (1.3 - shade) : colour * diffuse;
		fragment = clamp(fragment * 1.35, vec3(0.0), vec3(1.0));
	}

	// END
	FragColor = vec4(fragment * outline, 1.0f);
} 
