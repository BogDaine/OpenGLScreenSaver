#version 330 core

layout(location = 0) in vec4 position;
uniform vec4 u_Pos;

void main()
{
gl_Position = position;
gl_Position = gl_Position * u_Pos;
//gl_Position.y = -position.y;
}
