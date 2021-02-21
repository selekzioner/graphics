#pragma once

#include <QOpenGLShaderProgram>
#include <QColorDialog>
#include <QOpenGLWidget>
#include <memory>
#include "cube.h"
#include "cubeedges.h"

class MainWidget final : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    explicit MainWidget(QColorDialog& cDialog);

private:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

    void initializeGL() override;
    void paintGL() override;

    void initShaders();

    GLint m_posAttr = 0;
    GLint m_colAttr = 0;
    GLint m_matrixUniform = 0;

    std::shared_ptr<QOpenGLShaderProgram> m_program;

    enum class objectType { cube, edges };
    std::vector<std::unique_ptr<PlainObject3D>> objects;
    std::unique_ptr<PlainObject3D> objectFactory(objectType type);

    QColorDialog& c_dialog;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed = 0;
    QQuaternion rotation;

    void setRotation();
};
