#version 330

attribute vec4 posAttr;
attribute vec2 texAttr;
uniform mat4 matrix;

out vec2 texCoord;

void main()
{
  gl_Position = matrix * posAttr;
  texCoord = texAttr;
}
