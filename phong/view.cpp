#include "view.h"


QMatrix4x4 View::getViewMatrix() const
{
    QMatrix4x4 matrix;
    matrix.lookAt(pos_, pos_ + front_, up_);
    return matrix;
}


QVector3D View::getPos() const
{
    return pos_;
}
