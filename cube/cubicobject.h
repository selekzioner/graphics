#pragma once

#include "plainobject3d.h"

class CubicObject : public PlainObject3D
{
public:
    CubicObject() = delete;
    explicit CubicObject(GLfloat edgeLen, GLuint verticesCnt) : edgeLen_(edgeLen) {
        verticesCount = verticesCnt;
        vertices_.resize(verticesCount * 3);
        colors_.resize(verticesCount * 3);
    }

protected:
    GLfloat edgeLen_;
};
