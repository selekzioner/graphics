#pragma once

#include <QOpenGLWidget>
#include <QTime>
#include "cube.h"


class CubeWidget final : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit CubeWidget(QWidget* parent);

public slots:
    void setColor(const QColor& color);

    void setGrid(int value);
    void setMorph(int value);

    void setNumberOfCubes(int value);

signals:
    void updateFPS(const QString& info);

private:
    void initializeGL() override;
    void paintGL() override;

    void initShaders();
    void initCubes();

    QMatrix4x4 setMVPMatrix(const QVector3D& pos);

    QOpenGLShaderProgram m_program_;

    std::vector<std::unique_ptr<Cube>> cubes_;

    unsigned numberOfCubes_ = 1;
    GLfloat cubeSize_ = 2.f;

    unsigned gridStep_ = 1;
    bool isGridStepChanged_ = true;

    QColor color_ {255, 255, 255};

    float morphRate_ = 0;
    int frame_ = 0;

    QTime time_;
    unsigned frameCount_ = 0;
};
