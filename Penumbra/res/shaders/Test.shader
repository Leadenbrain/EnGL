#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Colour;

out vec3 v_Position;
out vec4 v_Colour;

uniform mat4 u_MVP; 

void main()
{
	v_Position =  a_Position;
	v_Colour = a_Colour;
	gl_Position = u_MVP*vec4(a_Position, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec3 v_Position;
in vec4 v_Colour;

void main()
{
	colour = v_Colour;
}