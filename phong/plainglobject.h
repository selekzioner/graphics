#pragma once
#include "GLObject.h"
#include <QColor>


class PlainGLObject : public GLObject
{
public:
    virtual void setColor(const QColor& color) = 0;
};
