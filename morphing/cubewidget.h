#pragma once

#include <QOpenGLWidget>
#include "cube.h"


class CubeWidget final : public QOpenGLWidget, protected QOpenGLFunctions
{
public slots:
    void setColor(const QColor& color);

    void setGrid(int value);
    void setMorph(int value);

private:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

    void initializeGL() override;
    void paintGL() override;

    void initShaders();

    QOpenGLShaderProgram m_program_;

    std::unique_ptr<Cube> cube;

    QColor color_ {255, 255, 255};
    float morphRate_ = 0;

    QVector2D mousePressPosition_;
    QVector3D rotationAxis_;
    qreal angularSpeed_ = 0;
    QQuaternion rotation_;

    void setRotation();
};
