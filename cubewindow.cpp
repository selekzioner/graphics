#include "cubewindow.h"
#include <QMouseEvent>


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

    setVertices();
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

    glVertexAttribPointer(m_posAttr, dim, GL_FLOAT, GL_FALSE, 0, vertices_.data());
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors_.data());

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 10);
    glDrawArrays(GL_TRIANGLE_STRIP, 10, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 14, 4);

    glDisableVertexAttribArray(m_colAttr);
    glDisableVertexAttribArray(m_posAttr);

    static GLfloat e_vertices[] = {
        -0.5f,   0.5f,	 0.5f,
         0.5f,	 0.5f,   0.5f,
         0.5f,	-0.5f,   0.5f,
        -0.5f,	-0.5f,   0.5f,

        -0.5f,	-0.5f,  -0.5f,
         0.5f,	-0.5f,  -0.5f,
         0.5f,	 0.5f,  -0.5f,
        -0.5f,	 0.5f,  -0.5f,

        -0.5f,   0.5f,	 0.5f,
        -0.5f,	 0.5f,  -0.5f,
        -0.5f,	-0.5f,  -0.5f,
        -0.5f,	-0.5f,   0.5f,

         0.5f,	 0.5f,   0.5f,
         0.5f,	-0.5f,   0.5f,
         0.5f,	-0.5f,  -0.5f,
         0.5f,	 0.5f,  -0.5f,
    };

    GLfloat e_color[54];
    for (auto i = 0; i < 54; i += 3){
        e_color[i] = 0.0f;
        e_color[i + 1] = 0.0f;
        e_color[i + 2] = 0.0f;
    }
    glVertexAttribPointer(m_posAttr, dim, GL_FLOAT, GL_FALSE, 0, e_vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, e_color);

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glDrawArrays(GL_LINE_LOOP, 4, 4);
    glDrawArrays(GL_LINE_LOOP, 8, 4);
    glDrawArrays(GL_LINE_LOOP, 12, 4);

    glDisableVertexAttribArray(m_colAttr);
    glDisableVertexAttribArray(m_posAttr);

    m_program->release();
}


void CubeWindow::setColor(const QColor& col)
{
    for (unsigned i = 0; i < colors_.size(); i += 3){
        auto coefficient = 0.05;
        coefficient = i % 3 == 0 ? coefficient / 10 : coefficient;

        colors_[i]     = col.redF() - i * coefficient;
        colors_[i + 1] = col.greenF() - i * coefficient;
        colors_[i + 2] = col.blueF() - i * coefficient;
    }
}


CubeWindow::~CubeWindow()
{
    delete m_program;
}


void CubeWindow::setVertices()
{
    GLfloat edgeLen = 0.5f;
    unsigned dim = 3;
    unsigned sideSurfCount = 10, topCount = 4, bottomCount = 4;

    Q_ASSERT(sideSurfCount + topCount + bottomCount == vertexCount);

    for (unsigned i = 0; i < sideSurfCount * dim; i += dim){               /* side surface vertices */
        auto idx = static_cast<int>(i / dim);
        vertices_[i]     = idx > 1 && idx < 6 ? edgeLen : -edgeLen;        // x
        vertices_[i + 1] = idx % 2 == 0 ? edgeLen : -edgeLen;              // y
        vertices_[i + 2] = idx > 3 && idx < 8 ? -edgeLen : edgeLen;        // z
    }

    auto topIdxEnd = topCount + sideSurfCount;
    for (unsigned i = sideSurfCount * dim; i < topIdxEnd * dim; i += dim){ /* top vertices */
        auto idx = static_cast<int>(i / dim) - sideSurfCount;
        vertices_[i]     = idx % 2 == 0 ? -edgeLen : edgeLen;              // x
        vertices_[i + 1] = edgeLen;                                        // y
        vertices_[i + 2] = idx > 1 ? -edgeLen : edgeLen;                   // z
    }

    auto bottomIdxEnd = topIdxEnd + bottomCount;
    for (unsigned i = topIdxEnd * dim; i < bottomIdxEnd * dim; i += dim) { /* bottom vertices */
        auto idx = static_cast<int>(i / dim) - topIdxEnd;
        vertices_[i]     = idx % 2 == 0 ? edgeLen : -edgeLen;              // x
        vertices_[i + 1] = -edgeLen;                                       // y
        vertices_[i + 2] = idx > 1 ? -edgeLen : edgeLen;                   // z
    }
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
