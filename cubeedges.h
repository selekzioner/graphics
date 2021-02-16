#ifndef CUBEEDGES_H
#define CUBEEDGES_H
#include "cubicobject.h"

class CubeEdges final: public CubicObject
{
public:
    CubeEdges() = delete;
    explicit CubeEdges(GLfloat edgeLen) : CubicObject(edgeLen, 16){}

    void setColor(const QColor& color) override;

private:
    void setVertices() override;
};

#endif // CUBEEDGES_H
