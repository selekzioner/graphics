#pragma once
#include "cube.h"

class CubicLighter final: public Cube
{
public:
    explicit CubicLighter(GLfloat edgeLen = 1, unsigned stepCount = 1);

    void setPos(const QVector3D& pos);
    QVector3D getPos() const;

    bool isEnabled = true;

private:
    QVector3D pos_{};

};
