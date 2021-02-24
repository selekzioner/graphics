#pragma once

#include "openglobject.h"
#include "square.h"


class Cube final: public OpenGLObject
{
public:
    explicit Cube(GLfloat edgeLen, unsigned stepCount);

    void initialize() override;
    void render(QOpenGLShaderProgram& program) override;

    void setColor(const QColor& color) override;

private:
    std::vector<Square> surfaces_;
};
