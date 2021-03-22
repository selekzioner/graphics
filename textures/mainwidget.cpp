#include "mainwidget.h"
#include <QKeyEvent>


MainWidget::MainWidget(QWidget* parent)
  : QOpenGLWidget(parent) {}


void MainWidget::initializeGL()
{
  initializeOpenGLFunctions();

  initShaders();
  initTextures();

  lighter_.initialize();
  earth_.initialize();

  setFocusPolicy(Qt::StrongFocus);

  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
}


void MainWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  QMatrix4x4 projection;
  projection.perspective(30.f, 1.7f, 0.1f, 100.0f);
  renderSphere(projection);
  renderLighter(projection);

  update();
  ++frame_;
}


void MainWidget::keyPressEvent(QKeyEvent* event)
{
  auto angle = 10.f;
  auto front = camera_.getFront();
  auto dir = camera_.getFront().z() / std::abs(camera_.getFront().z());

  QMatrix4x4 rotation;
  switch(event->key()){
    case Qt::Key_A:
      rotation.rotate(angle, 0.f, 1.f, 0.f);
      break;
    case Qt::Key_D:
      rotation.rotate(-angle, 0.f, 1.f, 0.f);
      break;
    case Qt::Key_W:
      rotation.rotate(-angle * dir, 1.f, 0.f, 0.f);
      break;
    case Qt::Key_S:
      rotation.rotate(angle * dir, 1.f, 0.f, 0.f);
      break;
    default:
      return;
  }
  front = rotation * front;
  camera_.setFront(front);
}


void MainWidget::wheelEvent(QWheelEvent* event)
{
  auto dr = 0.f;
  auto offset = 1.f;
  if (event->angleDelta().y() > 0){
    dr = offset * camera_.getFront().z() / abs(camera_.getFront().z());
  }
  else if (event->angleDelta().y() < 0){
    dr = -offset * camera_.getFront().z() / abs(camera_.getFront().z());
  }
  camera_.setPos({camera_.getPos().x() + dr * camera_.getFront().x(),
                  camera_.getPos().y() + dr * camera_.getFront().y(),
                  camera_.getPos().z() + dr * camera_.getFront().z()});
}


void MainWidget::initShaders()
{
  sShader_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/sphere.vsh");
  sShader_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/sphere.fsh");
  sShader_.link();

  lShader_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/lighter.vsh");
  lShader_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/lighter.fsh");
  lShader_.link();
}


void MainWidget::initTextures()
{
  earthTex_.reset(new QOpenGLTexture(QImage(":/Earth_Albedo.jpg")));
  earthTex_->setMinificationFilter(QOpenGLTexture::Nearest);

  sunTex_.reset(new QOpenGLTexture(QImage(":/Sun.jpg")));
  sunTex_->setMinificationFilter(QOpenGLTexture::Nearest);

  earthNormalMap_.reset(new QOpenGLTexture(QImage(":/Earth_NormalMap.jpg")));
  sunTex_->setMinificationFilter(QOpenGLTexture::Nearest);
}


void MainWidget::renderSphere(const QMatrix4x4& projection)
{
  sShader_.bind();

  QMatrix4x4 sModel;
  sModel.translate({ 0.f, 0.f, -4.f});
  sModel.rotate(frame_ / 2.f, 0, 1);
  sModel.rotate(-90.f, 1, 0, 0);

  sShader_.setUniformValue("model", sModel);
  sShader_.setUniformValue("view", camera_.getViewMatrix());
  sShader_.setUniformValue("lightPos", lighter_.getPos());
  sShader_.setUniformValue("viewPos", camera_.getPos());
  sShader_.setUniformValue("projection", projection);

  glActiveTexture(GL_TEXTURE0);
  earthTex_->bind();

  glActiveTexture(GL_TEXTURE1);
  earthNormalMap_->bind();

  sShader_.setUniformValue("ambCol", lighter_.getColor().ambient);
  sShader_.setUniformValue("diffCol", lighter_.getColor().diffuse);
  sShader_.setUniformValue("specCol", lighter_.getColor().specular);

  earth_.render(sShader_);
}


void MainWidget::renderLighter(const QMatrix4x4& projection)
{
  lShader_.bind();

  QMatrix4x4 lModel;
  lModel.translate(lighter_.getPos());
  lModel.rotate(frame_ / 2.f, 0, 1);
  lModel.rotate(-90.f, 1, 0, 0);

  lShader_.setUniformValue("matrix", projection * camera_.getViewMatrix() * lModel);
  glActiveTexture(GL_TEXTURE0);
  sunTex_->bind();

  lighter_.render(lShader_);
}
