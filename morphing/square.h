#pragma once

#include "openglobject.h"
#include <QOpenGLBuffer>


class Square final : public OpenGLObject
{
public:
    explicit Square(GLfloat edgeLen, unsigned stepCount);

    void initialize() override;
    void render(QOpenGLShaderProgram& program) override;

    void setColor(const QColor& color) override;

    void transform(const QMatrix4x4& matrix);

    ~Square();

private:
    const GLfloat edgeLen_ = 0.0f;
    unsigned stepCount_ = 0;

    std::vector<QVector3D> vertices_;
    std::vector<GLint> indices_;

    QOpenGLBuffer vertexBuffer_, indexBuffer_;

    QColor color_;

    void setVertices();
    void setIndices();
};
