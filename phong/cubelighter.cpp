#include "cubelighter.h"


CubicLighter::CubicLighter(GLfloat edgeLen, unsigned stepCount) : Cube(edgeLen, stepCount) {}


void CubicLighter::setPos(const QVector3D& pos)
{
    pos_ = pos;
}


QVector3D CubicLighter::getPos() const
{
    return pos_;
}
