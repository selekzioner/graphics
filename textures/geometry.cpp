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

  program.enableAttributeArray("tangent");
  program.setAttributeBuffer("tangent", GL_FLOAT, offsetof(Vertex, tangent), 3, sizeof(Vertex));

  program.enableAttributeArray("bitangent");
  program.setAttributeBuffer("bitangent", GL_FLOAT, offsetof(Vertex, bitangent), 3, sizeof(Vertex));

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

      /*if (vertices_.size() > 1) {
        auto edge1 = vertices_[vertices_.size() - 1].pos - vertices_[vertices_.size() - 2].pos;
        auto edge2 = vertex.pos - vertices_[vertices_.size() - 2].pos;
        auto deltaUV1 = vertices_[vertices_.size() - 1].texCoord - vertices_[vertices_.size() - 2].texCoord;
        auto deltaUV2 = vertex.texCoord - vertices_[vertices_.size() - 2].texCoord;
        float f = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV2.x() * deltaUV1.y());

        vertices_[vertices_.size() - 2].tangent = { f * (deltaUV2.y() * edge1.x() - deltaUV1.y() * edge2.x()),
                                                    f * (deltaUV2.y() * edge1.y() - deltaUV1.y() * edge2.y()),
                                                    f * (deltaUV2.y() * edge1.z() - deltaUV1.y() * edge2.z()) };

        vertices_[vertices_.size() - 2].bitangent = { f * (-deltaUV2.x() * edge1.x() + deltaUV1.x() * edge2.x()),
                                                      f * (-deltaUV2.x() * edge1.y() + deltaUV1.x() * edge2.y()),
                                                      f * (-deltaUV2.x() * edge1.z() + deltaUV1.x() * edge2.z()) };

      }*/
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
  for (auto i = 0u; i < indices_.size(); i += 3) {
    auto& vertex0 = vertices_[indices_[0u + i]];
    auto& vertex1 = vertices_[indices_[1u + i]];
    auto& vertex2 = vertices_[indices_[2u + i]];

    const auto Edge1 = vertex1.pos - vertex0.pos;
    const auto Edge2 = vertex2.pos - vertex0.pos;

    const auto DeltaU1 = vertex1.texCoord.x() - vertex0.texCoord.x();
    const auto DeltaV1 = vertex1.texCoord.y() - vertex0.texCoord.y();
    const auto DeltaU2 = vertex2.texCoord.x() - vertex0.texCoord.x();
    const auto DeltaV2 = vertex2.texCoord.y() - vertex0.texCoord.y();

    const auto f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

    QVector3D Tangent, Bitangent;

    Tangent.setX(f * (DeltaV2 * Edge1.x() - DeltaV1 * Edge2.x()));
    Tangent.setY(f * (DeltaV2 * Edge1.y() - DeltaV1 * Edge2.y()));
    Tangent.setZ(f * (DeltaV2 * Edge1.z() - DeltaV1 * Edge2.z()));

    Bitangent.setX(f * (-DeltaU2 * Edge1.x() + DeltaU1 * Edge2.x()));
    Bitangent.setY(f * (-DeltaU2 * Edge1.y() + DeltaU1 * Edge2.y()));
    Bitangent.setZ(f * (-DeltaU2 * Edge1.z() + DeltaU1 * Edge2.z()));

    vertex0.tangent = Tangent.normalized();
    vertex1.tangent = Tangent.normalized();
    vertex2.tangent = Tangent.normalized();

    vertex0.bitangent = Bitangent.normalized();
    vertex1.bitangent = Bitangent.normalized();
    vertex2.bitangent = Bitangent.normalized();

  }
}
