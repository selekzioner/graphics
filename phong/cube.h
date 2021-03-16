#pragma once

#include "plainobject.h"
#include "vertex.h"
#include <QOpenGLBuffer>

class Cube: public PlainObject
{
public:
    explicit Cube(GLfloat edgeLen, unsigned stepCount = 1);

    void initialize() override;
    void render(QOpenGLShaderProgram& program) override;

    ~Cube();

private:
    std::vector<Vertex> vertices_;
    std::vector<unsigned> indices_;

    GLfloat edgeLen_;
    unsigned stepCount_;

    QOpenGLBuffer vertexBuffer_, indexBuffer_;

    void setVertices(const QMatrix4x4& matrix);
};
