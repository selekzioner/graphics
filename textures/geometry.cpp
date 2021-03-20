#define _USE_MATH_DEFINES
#include <cmath>
#include <QColor>
#include "geometry.h"


Sphere::Sphere(GLfloat radius, GLuint lats, GLuint longs)
  : radius_(radius), lats_(lats), longs_(longs),
    vertexBuf_(QOpenGLBuffer::VertexBuffer),
    indexBuf_(QOpenGLBuffer::IndexBuffer) {}


void Sphere::initialize()
{
  initializeOpenGLFunctions();
  vertexBuf_.create(); indexBuf_.create();

  initVertices();
  initIndices();

  vertexBuf_.bind();
  vertexBuf_.allocate(vertices_.data(), vertices_.size() * sizeof(Vertex));
  indexBuf_.bind();
  indexBuf_.allocate(indices_.data(), indices_.size() * sizeof(GLuint));
}


void Sphere::render(QOpenGLShaderProgram& program)
{
  program.enableAttributeArray("posAttr");
  program.setAttributeBuffer("posAttr", GL_FLOAT, 0, 3, sizeof(Vertex));

  program.enableAttributeArray("normAttr");
  program.setAttributeBuffer("normAttr", GL_FLOAT, offsetof(Vertex, normal), 3, sizeof(Vertex));

  program.enableAttributeArray("texAttr");
  program.setAttributeBuffer("texAttr", GL_FLOAT, offsetof(Vertex, texCoord), 2, sizeof(Vertex));

  glPolygonMode(GL_FRONT, GL_FILL);
  glDrawElements(GL_TRIANGLES, indexBuf_.size(), GL_UNSIGNED_INT, nullptr);
};


Sphere::~Sphere()
{
  vertexBuf_.destroy();
  indexBuf_.destroy();
}


void Sphere::initVertices()
{
  auto latStep = 2 * M_PI / static_cast<float>(lats_);
  auto longStep = M_PI / static_cast<float>(longs_);
  Vertex vertex;

  for (auto i = 0u; i <= longs_; ++i) {
    auto stack_angle = M_PI / 2 - static_cast<float>(i) * longStep;
    auto xy = radius_ * cos(stack_angle);
    GLfloat z = radius_ * sin(stack_angle);

    for (auto j = 0u; j <= lats_; ++j) {
      GLfloat sector_angle = static_cast<float>(j) * latStep;

      GLfloat x = xy * cos(sector_angle);
      GLfloat y = xy * sin(sector_angle);
      vertex.pos = { x, y, z };
      vertex.normal = { x / radius_ , y / radius_, z / radius_ };

      vertex.texCoord = { static_cast<float>(j) / lats_ , static_cast<float>(i) / longs_ };

      vertices_.push_back(vertex);
    }
  }
}


void Sphere::initIndices()
{
  for (auto i = 0u; i < lats_; ++i){
    auto k1 = i * (longs_ + 1);
    auto k2 = k1 + longs_ + 1;

    for (auto j = 0u; j < longs_; ++j, ++k1, ++k2){
      if (i != 0){
        indices_.push_back(k1);
        indices_.push_back(k2);
        indices_.push_back(k1 + 1u);
      }

      if (i != (lats_ - 1)){
        indices_.push_back(k1 + 1u);
        indices_.push_back(k2);
        indices_.push_back(k2 + 1u);
      }
    }
  }
}
