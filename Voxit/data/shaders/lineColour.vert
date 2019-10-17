// LINE VERT
#version 330 core
layout (location = 0) in vec3 aPos; // VERTICE POSITION

uniform mat4 _model;		// LOCAL
uniform mat4 _view;			// WORLD
uniform mat4 _projection;	// CAMERA
uniform vec3 _colour;


out vec3 outColour;

void main()
{
    gl_Position =  _projection * _view * _model * vec4(aPos, 1.0);
	outColour = _colour;
}