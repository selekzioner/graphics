#pragma once
#include <QColor>
#include "geometry.h"

struct LightColor
{
  QColor ambient, diffuse, specular;
};

class Lighter final: public Sphere
{
public:
  Lighter();
  void setColor(const LightColor& col);

  QVector3D getPos() const;
  LightColor getColor() const;

private:
  QVector3D pos_{-10.f, 0.f, 20.f};
  LightColor col_{{0, 0, 255}, {255, 255, 100}, {255, 255, 255}};
};
