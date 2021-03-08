#pragma once
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>


class GLObject : protected QOpenGLFunctions
{
public:

    virtual void initialize() = 0;
    virtual void render(QOpenGLShaderProgram& program) = 0;

    virtual ~GLObject() = default;
};
