#pragma once

#include "cubicobject.h"

class CubeEdges final: public CubicObject
{
public:
    explicit CubeEdges(GLfloat edgeLen);

    void setColor(const QColor& color) override;

private:
    void setVertices() override;
    void drawArrays() override;
};
