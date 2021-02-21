#include "mainwidget.h"
#include <QMouseEvent>


MainWidget::MainWidget(QColorDialog& cDialog) : c_dialog(cDialog) {}


void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    initShaders();

    objects.push_back(objectFactory(objectType::cube));
    objects.push_back(objectFactory(objectType::edges));

    for (auto& object : objects){
        object->initialize();
    }

}


void MainWidget::initShaders()
{
    m_program = std::make_shared<QOpenGLShaderProgram>(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/vertex.vsh");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/fragment.fsh");
    m_program->link();

    m_matrixUniform = m_program->uniformLocation("matrix");
    Q_ASSERT(m_matrixUniform != -1);
}


void MainWidget::mousePressEvent(QMouseEvent *e)
{
    mousePressPosition = QVector2D(e->localPos());
}


void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();       /* Rotation axis is perpendicular to the mouse position difference vector */

    qreal acc = diff.length() / 100.0;                                   /* Accelerate angular speed relative to the length of the mouse sweep */

    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized(); /* Calculate new rotation axis as weighted sum */
    angularSpeed += acc;
}


void MainWidget::paintGL()
{
    //

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2);

    setRotation();
    matrix.rotate(rotation);

    m_program->setUniformValue(m_matrixUniform, matrix);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    for (auto& object : objects){
        object->setColor(c_dialog.currentColor());
        object->render(m_program);
    }

    m_program->release();
    update();
}


std::unique_ptr<PlainObject3D> MainWidget::objectFactory(objectType type)
{
    switch(type) {
    case objectType::cube:
        return std::make_unique<Cube>(1.0);
    default:
        return std::make_unique<CubeEdges>(1.0);
    }
}


void MainWidget::setRotation()
{
    angularSpeed *= 0.99;

    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    }
    else {
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
    }
}
