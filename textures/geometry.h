#pragma once
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

struct Vertex
{
  QVector3D pos, normal;
  QVector2D texCoord;
};

class Sphere : protected QOpenGLFunctions
{
public:
  explicit Sphere(GLfloat radius, GLuint lats, GLuint longs);

  void initialize();
  void render(QOpenGLShaderProgram& program);

  ~Sphere();

private:
  void initVertices();
  void initIndices();

  GLfloat radius_;
  GLuint lats_, longs_;
  QOpenGLBuffer vertexBuf_, indexBuf_;
  std::vector<Vertex> vertices_;
  std::vector<GLuint> indices_;
};
