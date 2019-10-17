// SELECTOR FRAG
#version 330 core

in vec2 objSize;
in vec2 texCoord;
in vec4 colour;

out vec4 FragColor;

void main()
{
	float radiusX = 1.0f - abs(0.10f / -objSize.x);
	float radiusY = 1.0f - abs(0.10f / -objSize.y);

	float x = step(texCoord.x, radiusX);
	float y = step(texCoord.y, radiusY);
	float dx = step(1.0f - texCoord.x, radiusX);
	float dy = step(1.0f - texCoord.y, radiusY);
	
	float sum = x*dx*y*dy;
    FragColor = vec4(vec3(1.0f) * sum, 1.0f - sum); //*/ 
} 