#pragma once

#include <QOpenGLWidget>
#include <QOpenGLTexture>
#include "geometry.h"
#include "lighter.h"
#include "camera.h"

class MainWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT

public:
  MainWidget(QWidget* parent = nullptr);

protected:
  void initializeGL() override;
  void paintGL() override;

  void keyPressEvent(QKeyEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;

private:
  void initShaders();
  void initTextures();

  void renderSphere(const QMatrix4x4& projection);
  void renderLighter(const QMatrix4x4& projection);

  QOpenGLShaderProgram sShader_, lShader_;

  Sphere earth_{ 0.5f, 100, 100 }, mars_{0.2f, 10, 100};
  QVector3D earthPos_{ 0.f, 0.f, -4.f};

  Lighter lighter_;
  Camera camera_;
  std::unique_ptr<QOpenGLTexture> earthTex_, earthNormalMap_,
                                  marsTex_,  marsNormalMap_, sunTex_;

  unsigned frame_ = 0;
};
