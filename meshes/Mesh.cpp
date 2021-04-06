#include "Mesh.h"

Mesh::Mesh()
: _vertexBuf(QOpenGLBuffer::VertexBuffer),
		_indexBuf(QOpenGLBuffer::IndexBuffer)
{
}

Mesh::~Mesh()
{
  _vertexBuf.destroy();
  _indexBuf.destroy();
}

void Mesh::Initialize()
{
  initializeOpenGLFunctions();

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
	
  _vertexBuf.create();
	_indexBuf.create();

  _vertexBuf.bind();
  _vertexBuf.allocate(vertices.data(), static_cast<int>(vertices.size() * sizeof(Vertex)));
  _indexBuf.bind();
  _indexBuf.allocate(indices.data(), static_cast<int>(indices.size() * sizeof(unsigned)));
}

void Mesh::Render(QOpenGLShaderProgram& program)
{
  _vertexBuf.bind();
  _indexBuf.bind();
	
  program.enableAttributeArray("posAttr");
  program.setAttributeBuffer("posAttr", GL_FLOAT, 0, 3, sizeof(Vertex));

  program.enableAttributeArray("normAttr");
  program.setAttributeBuffer("normAttr", GL_FLOAT, offsetof(Vertex, normal), 3, sizeof(Vertex));

  program.enableAttributeArray("texAttr");
  program.setAttributeBuffer("texAttr", GL_FLOAT, offsetof(Vertex, texCoord), 2, sizeof(Vertex));

  if (texture) {
    program.setUniformValue("Texture", 0);
    glActiveTexture(GL_TEXTURE0);
    texture->bind();
  }

  if (normalMap) {
    program.setUniformValue("normalMap", 1);
    glActiveTexture(GL_TEXTURE1);
    normalMap->bind();
  }

  program.setUniformValue("diffCol", material.diffuse);
  program.setUniformValue("specCol", material.specular);

  glPolygonMode(GL_FRONT, GL_FILL);
  if (mode == GLMode::TRIANGLES) {
    glDrawElements(GL_TRIANGLES, _indexBuf.size(), GL_UNSIGNED_INT, nullptr);
  } else {
    glDrawElements(GL_QUADS, _indexBuf.size(), GL_UNSIGNED_INT, nullptr);
  }

  if (texture) {
    texture->release();
  }
  if (normalMap) {
    normalMap->release();
  }
}