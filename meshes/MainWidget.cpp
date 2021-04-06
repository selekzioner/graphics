#include "MainWidget.h"
#include "Sphere.h"

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

  ObjectPtr sun = std::make_shared<Sphere>("Sun");
  sun->SetPos({ -3.f, 0.f, -5.f });
  sun->SetRotation({ 0.f, 1.f, 0.f, 0.3f });
	
  ObjectPtr earth = std::make_shared<Sphere>("Earth");
  earth->SetPos({ 0.f, 0.f, -1.f });
  earth->SetRotation({ 0.f, 1.f, 0.f, 0.3f });

  ObjectPtr mars = std::make_shared<Sphere>("Mars");
  mars->SetPos({ 1.f, 0.f, -2.f });
  mars->SetRotation({ 0.f, 1.f, 0.f, 0.3f });

  auto cube = std::make_shared<Object>("Cube");
  cube->SetPos({ 3.f, 0.f, -1.f });
  cube->SetRotation({ 0.f, 1.f, 0.f, 0.3f });

  _objects.push_back(std::move(sun));
  _objects.push_back(std::move(earth));
  _objects.push_back(std::move(mars));
  _objects.push_back(std::move(cube));

	for (auto& obj : _objects) {
    obj->Initialize();
	}

  setFocusPolicy(Qt::StrongFocus);
}

void MainWidget::paintGL()
{
  QMatrix4x4 projection;
  projection.perspective(30.f, 1.7f, 0.1f, 100.0f);
  _shader.setUniformValue("view", _camera.getViewMatrix());
  _shader.setUniformValue("viewPos", _camera.pos);
  _shader.setUniformValue("projection", projection);
	
  for (auto& obj : _objects) {
    if (obj->Name() == "Sun") {
      _shader.setUniformValue("lightPos", obj->GetPos());
      _shader.setUniformValue("isLighter", 1);
    } else {
      _shader.setUniformValue("isLighter", 0);
    }
  	
    obj->Render(_shader);
  }
  update();
}

void MainWidget::keyPressEvent(QKeyEvent* event)
{
  const auto angle = 5.f;
  const auto front = _camera.front;
  const auto dir = _camera.front.z() / std::abs(_camera.front.z());

  QMatrix4x4 rotation;
  switch (event->key()) {
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