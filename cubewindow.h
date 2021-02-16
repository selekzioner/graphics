#ifndef CUBEWINDOW_H
#define CUBEWINDOW_H

#include <QPushButton>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QColorDialog>
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

    constexpr static const unsigned vertexCount = 18;       /* for cube consists of 3 trianglestrip */
    constexpr static const unsigned dim = 3;

    std::array<GLfloat, vertexCount * dim> vertices_;
    std::array<GLfloat, vertexCount * 3> colors_;

    QColorDialog* c_dialog;

    void setVertices();

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed = 0;
    QQuaternion rotation;

    void setRotation();
};
#endif // CUBEWINDOW_H
