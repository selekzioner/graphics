#ifndef CUBE_H
#define CUBE_H
#include "cubicobject.h"

class Cube final: public CubicObject
{
public:
    Cube() = delete;
    explicit Cube(GLfloat edgeLen) : CubicObject(edgeLen, 18){} /* 18 vertices for cube consists of 3 trianglestrip */

    void setColor(const QColor& color) override;
private:
    void setVertices() override;
};

#endif // CUBE_H
