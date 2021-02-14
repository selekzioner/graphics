#ifndef CUBEWINDOW_H
#define CUBEWINDOW_H

#include <QPushButton>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include "openglwindow.h"

class CubeWindow : public OpenGLWindow
{
public:
    using OpenGLWindow::OpenGLWindow;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;


    void initialize() override;
    void render() override;

    void setColor(const QColor& col);

    ~CubeWindow();

private:
    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;

    QOpenGLShaderProgram *m_program = nullptr;

    QColor color;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed = 0;
    QQuaternion rotation;

    void setRotation();
};
#endif // CUBEWINDOW_H
