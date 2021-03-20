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
  projection.perspective(30.f, 1.05f, 0.1f, 100.0f);
  renderSphere(projection);
  renderLighter(projection);

  update();
  ++frame_;
}


void MainWidget::keyPressEvent(QKeyEvent* event)
{
  auto dx = 0.f, dy = 0.f;
  auto offset = 0.1f;
  switch(event->key()){
    case Qt::Key_W:
      dy = offset;
      break;
    case Qt::Key_S:
      dy = -offset;
      break;
    case Qt::Key_A:
      dx = offset * camera_.getFront().z() / std::abs(camera_.getFront().z());
      break;
    case Qt::Key_D:
      dx = -offset * camera_.getFront().z() / std::abs(camera_.getFront().z());
      break;
  }
  camera_.setPos({camera_.getPos().x() + dx,
                  camera_.getPos().y() + dy,
                  camera_.getPos().z()});
}


void MainWidget::mousePressEvent(QMouseEvent* event)
{
  auto geom = geometry();

  auto tol = 100.f;
  auto angle = 15.f;
  auto front = camera_.getFront();

  QMatrix4x4 rotationX;
  if (event->pos().x() < geom.left() + tol){
    rotationX.rotate(angle, 0.f, 1.f, 0.f);
    front = rotationX * front;
    camera_.setFront(front);
  }
  else if (event->pos().x() > geom.right() - tol){
    rotationX.rotate(-angle, 0.f, 1.f, 0.f);
    front = rotationX * front;
    camera_.setFront(front);
  }

  QMatrix4x4 rotationY;
  auto pos = event->pos().y();
  auto bot = geom.bottom() + tol;
  auto dir = camera_.getFront().z() / std::abs(camera_.getFront().z());
  if (event->pos().y() < geom.top() + tol){
    rotationY.rotate(-angle * dir, 1.f, 0.f, 0.f);
    front = rotationY * front;
    camera_.setFront(front);
  }
  else if (event->pos().y() > geom.bottom() - tol){
    rotationY.rotate(angle * dir, 1.f, 0.f, 0.f);
    front = rotationY * front;
    camera_.setFront(front);
  }
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
  texture_.reset(new QOpenGLTexture(QImage(":/Earth_Albedo.jpg")));
  texture_->setMinificationFilter(QOpenGLTexture::Nearest);
  texture_->setMagnificationFilter(QOpenGLTexture::Linear);
  texture_->setWrapMode(QOpenGLTexture::Repeat);

  normalMap_.reset(new QOpenGLTexture(QImage(":/Earth_NormalMap.jpg")));
}


void MainWidget::renderSphere(const QMatrix4x4& projection)
{
  sShader_.bind();

  QMatrix4x4 sModel;
  sModel.translate({ 0.f, 0.f, -4.f});
  sModel.rotate(frame_ / 2.f, 0, 1);
  sModel.rotate(270.f, 1, 0, 0);

  sShader_.setUniformValue("model", sModel);
  sShader_.setUniformValue("view", camera_.getViewMatrix());
  sShader_.setUniformValue("lightPos", lighter_.getPos());
  sShader_.setUniformValue("viewPos", camera_.getPos());
  sShader_.setUniformValue("projection", projection);
  texture_->bind();
  sShader_.setUniformValue("texture", 0);
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

  lShader_.setUniformValue("matrix", projection * camera_.getViewMatrix() * lModel);
  lShader_.setUniformValue("col",  lighter_.getColor().diffuse);

  lighter_.render(lShader_);
}
