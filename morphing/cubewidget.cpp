#include "cubewidget.h"
#include <QMouseEvent>


void CubeWidget::setColor(const QColor& color)
{
    color_ = color;
}


void CubeWidget::setGrid(int value)
{
    cube.reset(new Cube(2.0f, value));
    cube->initialize();
}


void CubeWidget::setMorph(int value)
{
    morphRate_ = value / 10.0f;
}


void CubeWidget::initializeGL()
{
    initializeOpenGLFunctions();
    initShaders();

    cube.reset(new Cube(2.0f, 1));
    cube->initialize();

    glClearColor(1.0f, 1.0f, 1.0f, 1000.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}


void CubeWidget::initShaders()
{
    if (!m_program_.addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/vertex.vsh")) {
        close();
    }
    if (!m_program_.addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/fragment.fsh")) {
        close();
    }
    if (!m_program_.link()) {
        close();
    }
    if (!m_program_.bind()) {
        close();
    }
}


void CubeWidget::mousePressEvent(QMouseEvent *e)
{
    mousePressPosition_ = QVector2D(e->localPos());
}


void CubeWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition_;
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();          /* Rotation axis is perpendicular to the mouse position difference vector */

    qreal acc = diff.length() / 100.0;                                      /* Accelerate angular speed relative to the length of the mouse sweep */

    rotationAxis_ = (rotationAxis_ * angularSpeed_ + n * acc).normalized(); /* Calculate new rotation axis as weighted sum */
    angularSpeed_ += acc;
}


void CubeWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -3.5);

    setRotation();
    matrix.rotate(rotation_);

    m_program_.setUniformValue("matrix", matrix);
    m_program_.setUniformValue("morphRate", morphRate_);

    cube->setColor(color_);

    cube->render(m_program_);
    update();
}


void CubeWidget::setRotation()
{
    angularSpeed_ *= 0.99;

    if (angularSpeed_ < 0.01) {
        angularSpeed_ = 0.0;
    }
    else {
        rotation_ = QQuaternion::fromAxisAndAngle(rotationAxis_, angularSpeed_) * rotation_;
    }
}
