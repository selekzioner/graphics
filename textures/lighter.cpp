#include "lighter.h"


Lighter::Lighter() : Sphere(1.f, 20, 20) {}


void Lighter::setColor(const LightColor& col)
{
  col_ = col;
}


QVector3D Lighter::getPos() const
{
  return pos_;
}


LightColor Lighter::getColor() const
{
  return col_;
}
