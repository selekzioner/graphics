#pragma once
#include <QMatrix4x4>

class transformableInterface
{
public:
    virtual void setPerspective(const QVector4D& vec) = 0;
    virtual void setTranslate(const QVector3D& vec) = 0;
    virtual void setRotate(const QVector4D& vec) = 0;
};


class transformable : public transformableInterface
{
public:
    void setPerspective(const QVector4D& vec) override;
    void setTranslate(const QVector3D& vec) override;
    void setRotate(const QVector4D& vec) override;

protected:
    QMatrix4x4 transformMatrix_;
};
