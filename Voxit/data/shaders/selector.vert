// SELECTOR VERT
#version 330 core

layout (location = 0) in vec3 aPos; // VERTICE POSITION
layout (location = 1) in vec2 aTex; // TEXTURE COORDINATES
layout (location = 2) in vec2 aDom; // DOMINATE AXIS

uniform mat4 _model;		// LOCAL
uniform mat4 _view;			// WORLD
uniform mat4 _projection;	// CAMERA
uniform vec4 _colour;		// COLOUR

uniform vec3 _start;
uniform vec3 _end;

out vec2 objSize;
out vec2 texCoord;
out vec4 colour;

void main()
{
	vec3 position = aPos; // VERTICES
	vec3 length = vec3(	// LENGTH BETWEEN _start & _end
		(abs(_start.x - _end.x)) + 1.05f,
		(abs(_start.y - _end.y)) + 1.05f,
		(abs(_start.z - _end.z)) + 1.05f
	);
	
	position *= length;
	
	mat4 model = _model; // MODEL POSITION
	model[3][0] = (_start.x / 2) + (_end.x / 2);
	model[3][1] = (_start.y / 2) + (_end.y / 2);
	model[3][2] = (_start.z / 2) + (_end.z / 2);
	
	colour  = _colour;
	texCoord = aTex;
	objSize = vec2(
		mix(length.x, length.y, aDom.x),
		mix(length.y, length.z, aDom.y)
	);
	
    gl_Position =  _projection * _view * model * vec4(position, 1.0);
}