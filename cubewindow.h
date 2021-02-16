#ifndef CUBEWINDOW_H
#define CUBEWINDOW_H

#include <QOpenGLShaderProgram>
#include <QColorDialog>
#include "openglwindow.h"
#include "cube.h"
#include "cubeedges.h"

class CubeWindow : public OpenGLWindow
{
public:
    using OpenGLWindow::OpenGLWindow;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

    void initialize() override;
    void render() override;

    ~CubeWindow();

private:
    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;

    QOpenGLShaderProgram *m_program = nullptr;

    Cube cube {1.0f};
    CubeEdges edges {1.0f};

    QColorDialog* c_dialog;

    void renderSurfaces();
    void renderEdges();

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed = 0;
    QQuaternion rotation;

    void setRotation();
};
#endif // CUBEWINDOW_H