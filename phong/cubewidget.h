#pragma once

#include <QOpenGLWidget>
#include <QTime>
#include "cubelighter.h"
#include "cube.h"
#include "view.h"


class CubeWidget final : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit CubeWidget(QWidget* parent);

public slots:
    void setCubesColor(const QColor& color);
    void setNumberOfCubes(int value);

    void setAmbientColor(const QColor& color);
    void setPointLightColor(const QColor& color);
    void setDirectedLightColor(const QColor& color);
    void setProjectorColor(const QColor& color);

    void setPointLightX(double x);
    void setPointLightY(double y);
    void setPointLightZ(double z);

    void setDirectedLightX(double x);
    void setDirectedLightY(double y);
    void setDirectedLightZ(double z);

    void setProjectorLightX(double x);
    void setProjectorLightY(double y);
    void setProjectorLightZ(double z);

    void setAmbientCoef(int value);
    void setDiffuseCoef(int value);
    void setSpecularCoef(int value);

    void switchAmbient();
    void switchPointLight();
    void switchDirectedLight();
    void switchProjector();

signals:
    void updateFPS(const QString& info);

private:
    void initializeGL() override;
    void paintGL() override;

    void initShaders();
    void initCubes();

    QOpenGLShaderProgram objectShader_, lighterShader_;

    std::vector<std::unique_ptr<Cube>> cubes_;

    unsigned numberOfCubes_ = 1;
    GLfloat cubeSize_ = 2.f;

    View view_;

    unsigned gridStep_ = 1;
    bool isGridStepChanged_ = true;

    QColor ambientColor_ { 255, 0, 0 };
    bool isAmbientEnabled_ = true;

    CubicLighter directedLight_,
                 pointLight_,
                 projectorLight_;

    GLfloat ka_ = 0.3f, kd_ = 1.f, ks_ = 0.5f;

    float morphRate_ = 0;
    int frame_ = 0;

    QTime time_;
    unsigned frameCount_ = 0;
};
