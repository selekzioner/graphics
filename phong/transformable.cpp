#include "transformable.h"


void transformable::setPerspective(const QVector4D& vec)
{
    transformMatrix_.perspective(vec[0], vec[1], vec[2], vec[3]);
}


void transformable::setTranslate(const QVector3D& vec)
{
    transformMatrix_.translate(vec);
}


void transformable::setRotate(const QVector4D& vec)
{
    transformMatrix_.rotate(vec[0], vec[1], vec[2], vec[3]);
}
