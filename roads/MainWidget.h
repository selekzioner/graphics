#pragma once
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>

#include "Camera.h"
#include "Object.h"

class MainWidget final : public QOpenGLWidget {
  Q_OBJECT

public:
  explicit MainWidget(QWidget* parent = nullptr);

protected:
  void initializeGL() override;
  void paintGL() override;

  void keyPressEvent(QKeyEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;

private:
  static void setFilter(ObjectPtr& obj, unsigned idx);
	
  QOpenGLShaderProgram _shader;

  std::vector<ObjectPtr> _objects;
  Camera _camera;
};

