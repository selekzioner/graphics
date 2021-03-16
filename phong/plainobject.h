#pragma once
#include "GLObject.h"
#include <QColor>


class PlainObject : public GLObject
{
public:
    virtual void setColor(const QColor& color);
    virtual QColor getColor() const;

protected:
    QColor color_ { 255, 255, 255 };
};
