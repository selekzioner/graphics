#version 330

attribute vec4 posAttr;
uniform mat4 matrix;

void main()
{
    gl_Position = matrix * posAttr;
}
