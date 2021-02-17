#include "plainobject3d.h"


void PlainObject3D::initialize()
{
    setVertices();
}


GLfloat const* PlainObject3D::getVertices() const
{
    return vertices_.data();
}


GLfloat const* PlainObject3D::getColors() const
{
    return colors_.data();
}
