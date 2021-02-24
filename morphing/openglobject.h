#pragma once
#include <QOpenGLFunctions>
#include <QColor>
#include <QOpenGLShaderProgram>


class OpenGLObject : protected QOpenGLFunctions
{
public:

    virtual void initialize() = 0;
    virtual void render(QOpenGLShaderProgram& program) = 0;

    virtual void setColor(const QColor& color) = 0;

    virtual ~OpenGLObject() = default;
};
