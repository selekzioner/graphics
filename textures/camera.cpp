#include "camera.h"


void Camera::setPos(const QVector3D& pos)
{
  pos_ = pos;
}


void Camera::setFront(const QVector3D& front)
{
  front_ = front;
}


void Camera::setUp(const QVector3D& up)
{
  up_ = up;
}


QMatrix4x4 Camera::getViewMatrix() const
{
  QMatrix4x4 matrix;
  matrix.lookAt(pos_, pos_ + front_, up_);
  return matrix;
}


QVector3D Camera::getPos() const
{
  return pos_;
}


QVector3D Camera::getFront() const
{
  return front_;
}


QVector3D Camera::getUp() const
{
  return up_;
}
