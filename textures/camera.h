#pragma once
#include <QVector3D>
#include <QMatrix4x4>

class Camera
{
public:
  void setPos(const QVector3D& pos);
  void setFront(const QVector3D& front);
  void setUp(const QVector3D& up);

  QMatrix4x4 getViewMatrix() const;

  QVector3D getPos() const;
  QVector3D getFront() const;
  QVector3D getUp() const;

private:
  QVector3D pos_  { 0.f,  0.f,  0.f },
            front_{ 0.f,  0.f, -1.f },
            up_   { 0.f,  1.f,  0.f };
};
