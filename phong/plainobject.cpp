#include "plainobject.h"


void PlainObject::setColor(const QColor& color)
{
    color_ = color;
}


QColor PlainObject::getColor() const
{
    return color_;
}
