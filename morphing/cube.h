#pragma once

#include "cubicobject.h"

class Cube final: public CubicObject
{
public:
    explicit Cube(GLfloat edgeLen);

    void setColor(const QColor& color) override;
private:
    void setVertices() override;
    void drawArrays() override;
};
