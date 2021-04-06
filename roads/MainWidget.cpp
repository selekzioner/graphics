#include "MainWidget.h"
#include "Rect.h"

#include <QKeyEvent>

MainWidget::MainWidget(QWidget* parent)
: QOpenGLWidget(parent)
{
}

void MainWidget::initializeGL()
{
  _shader.addShaderFromSourceFile(QOpenGLShader::Vertex, "Shaders/object.vsh");
  _shader.addShaderFromSourceFile(QOpenGLShader::Fragment, "Shaders/object.fsh");
  _shader.link();
  _shader.bind();

  const auto count = 4u;
  const auto mult = 1.05f;
  const auto strt = -1.5f;
	
  for (auto i = 0u; i < count; i++) {
    ObjectPtr road1 = std::make_shared<Rect>("road_1");
    road1->SetPos({ -mult*count + mult * i, 0, 0 });
    setFilter(road1, i);
    _objects.push_back(std::move(road1));
  }

  for (auto i = 0u; i < count; i++) {
    ObjectPtr road2 = std::make_shared<Rect>("road_2");
    road2->SetPos({  mult * i, 0, 0 });
    setFilter(road2, i);
    _objects.push_back(std::move(road2));
  }

  for (auto i = 0u; i < count; i++) {
    ObjectPtr road3 = std::make_shared<Rect>("road_3");
    road3->SetPos({ mult * count + mult * i, 0, 0 });
    setFilter(road3, i);
    _objects.push_back(std::move(road3));
  }

  setFocusPolicy(Qt::StrongFocus);
}

void MainWidget::paintGL()
{
  QMatrix4x4 projection;
  projection.perspective(60.0f, 4.0f / 3.0f, 0.1f, 30.0f);
  _shader.setUniformValue("view", _camera.getViewMatrix());
  _shader.setUniformValue("viewPos", _camera.pos);
  _shader.setUniformValue("projection", projection);
	
  for (auto& obj : _objects) {
    obj->Render(_shader);
  }
  update();
}

void MainWidget::keyPressEvent(QKeyEvent* event)
{
  const auto angle = 5.f;
  const auto front = _camera.front;
  const auto dir = _camera.front.z() / std::abs(_camera.front.z());
  const auto dy = 0.1f;

  QMatrix4x4 rotation;
  switch (event->key()) {
  case Qt::Key_A:
    rotation.rotate(angle, 0.f, 1.f, 0.f);
    break;
  case Qt::Key_D:
    rotation.rotate(-angle, 0.f, 1.f, 0.f);
    break;
  case Qt::Key_W:
    _camera.pos.setY(_camera.pos.y() + dy);
    break;
  case Qt::Key_S:
    _camera.pos.setY(_camera.pos.y() - dy);
    break;
  default:
    return;
  }
  _camera.front = rotation * front;
}

void MainWidget::wheelEvent(QWheelEvent* event)
{
  auto dr = 0.f;
  const auto offset = 1.f;
  if (event->angleDelta().y() > 0) {
    dr = offset * _camera.front.z() / abs(_camera.front.z());
  }
  else if (event->angleDelta().y() < 0) {
    dr = -offset * _camera.front.z() / abs(_camera.front.z());
  }
  _camera.pos = { _camera.pos.x() + dr * _camera.front.x(),
                  _camera.pos.y() + dr * _camera.front.y(),
                  _camera.pos.z() + dr * _camera.front.z() };
}

void MainWidget::setFilter(ObjectPtr& obj, const unsigned idx)
{
	switch (idx) {
  case 0:
    obj->Initialize(QOpenGLTexture::Nearest, 
      QOpenGLTexture::Nearest, false);
    break;
  case 1:
    obj->Initialize(QOpenGLTexture::LinearMipMapNearest, 
      QOpenGLTexture::LinearMipMapNearest, false);
    break;
  case 2:
    obj->Initialize(QOpenGLTexture::LinearMipMapLinear, 
      QOpenGLTexture::Linear, false);
    break;
  case 3:
    obj->Initialize(QOpenGLTexture::LinearMipMapLinear, 
      QOpenGLTexture::Linear, true);
    break;
  default:
    break;
	}
}