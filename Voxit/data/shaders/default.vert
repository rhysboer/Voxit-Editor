// DEFAULT VERT
#version 330 core
layout (location = 0) in vec3 aPos; // VERTICE POSITION
layout (location = 1) in vec3 aColour;

uniform mat4 _model;		// LOCAL
uniform mat4 _view;			// WORLD
uniform mat4 _projection;	// CAMERA

out vec3 colour;

void main()
{
    gl_Position =  _projection * _view * _model * vec4(aPos, 1.0);
	colour  = aColour;
}