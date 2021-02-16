#include "cubewindow.h"
#include <QMouseEvent>
#include <QToolButton>


static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";


void CubeWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();

    m_posAttr = m_program->attributeLocation("posAttr");
    Q_ASSERT(m_posAttr != -1);
    m_colAttr = m_program->attributeLocation("colAttr");
    Q_ASSERT(m_colAttr != -1);
    m_matrixUniform = m_program->uniformLocation("matrix");
    Q_ASSERT(m_matrixUniform != -1);

    cube.initialize();
    edges.initialize();

    c_dialog = new QColorDialog();
    c_dialog->setOptions(QColorDialog::NoButtons);
    c_dialog->setVisible(1);
}


void CubeWindow::mousePressEvent(QMouseEvent *e)
{
    mousePressPosition = QVector2D(e->localPos());
}


void CubeWindow::mouseReleaseEvent(QMouseEvent *e)
{
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();       /* Rotation axis is perpendicular to the mouse position difference vector */

    qreal acc = diff.length() / 100.0;                                   /* Accelerate angular speed relative to the length of the mouse sweep */

    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized(); /* Calculate new rotation axis as weighted sum */
    angularSpeed += acc;
}


void CubeWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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

    cube.setColor(c_dialog->currentColor());

    renderSurfaces();
    renderEdges();

    m_program->release();
}


CubeWindow::~CubeWindow()
{
    delete m_program;
    delete c_dialog;
}


void CubeWindow::renderSurfaces()
{
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, cube.getVertices());
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, cube.getColors());

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 10);
    glDrawArrays(GL_TRIANGLE_STRIP, 10, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 14, 4);

    glDisableVertexAttribArray(m_colAttr);
    glDisableVertexAttribArray(m_posAttr);
}


void CubeWindow::renderEdges()
{
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, edges.getVertices());
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, edges.getColors());

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    for (auto i = 0; i <= 12; i += 4){
        glDrawArrays(GL_LINE_LOOP, i, 4);
    }

    glDisableVertexAttribArray(m_colAttr);
    glDisableVertexAttribArray(m_posAttr);
}


void CubeWindow::setRotation()
{
    angularSpeed *= 0.99;

    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    }
    else {
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
    }
}
