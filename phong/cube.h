#pragma once

#include "plainglobject.h"
#include "square.h"
#include <QMatrix4x4>


class Cube final: public PlainGLObject
{
public:
    explicit Cube(GLfloat edgeLen, unsigned stepCount);

    void initialize() override;
    void render(QOpenGLShaderProgram& program) override;

    void setColor(const QColor& color) override;

private:
    std::vector<Square> surfaces_;
};
