#pragma once
#include <QVector3D>
#include <QMatrix4x4>

class View
{
public:
    QMatrix4x4 getViewMatrix() const;
    QVector3D getPos() const;

private:
    QVector3D pos_  { 0.f,  30.f,  20.f },
              front_{ 0.f,  -0.5f, -1.f },
              up_   { 0.f,  1.f,  0.f };
};
