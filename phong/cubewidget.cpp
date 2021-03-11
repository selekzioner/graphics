#include "cubewidget.h"
#include <QMouseEvent>


CubeWidget::CubeWidget(QWidget* parent) : QOpenGLWidget(parent) {}


void CubeWidget::setColor(const QColor& color)
{
    color_ = color;
}


void CubeWidget::setGrid(int value)
{
    gridStep_ = value;
    isGridStepChanged_ = true;
    initCubes();
}


void CubeWidget::setMorph(int value)
{
    morphRate_ = value / 10.0f;
}


void CubeWidget::setNumberOfCubes(int value)
{
    numberOfCubes_ = value != 1 ? 2 * value : 1;
    initCubes();
}


void CubeWidget::initializeGL()
{
    initializeOpenGLFunctions();
    initShaders();
    initCubes();

    glClearColor(1.0f, 1.0f, 1.0f, 1000.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    time_ = QTime::currentTime();
}


void CubeWidget::initShaders()
{
    if (!m_program_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex.vsh")) {
        close();
    }
    if (!m_program_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment.fsh")) {
        close();
    }
    if (!m_program_.link()) {
        close();
    }
    if (!m_program_.bind()) {
        close();
    }
}


void CubeWidget::initCubes()
{
    auto prevSize = cubes_.size();
    cubes_.resize(numberOfCubes_);
    if (isGridStepChanged_){
        isGridStepChanged_ = false;
        prevSize = 0;
    }
    if (prevSize >= numberOfCubes_){
        return;
    }
    std::for_each(cubes_.begin() + prevSize, cubes_.end(), [&](std::unique_ptr<Cube>& cube){
        cube.reset(new Cube(cubeSize_, gridStep_));
        cube->initialize();
    });
}


void CubeWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_program_.setUniformValue("morphRate", morphRate_);

    const auto rows = static_cast<int>(cubes_.size() / std::sqrt(cubes_.size()));
    const auto cols = static_cast<int>(cubes_.size() / rows);

    for (auto i = 0; i < cols; ++i){
        for (auto j = 0; j < rows; ++j){
            m_program_.setUniformValue("matrix", setMVPMatrix({ - 1.5f * cubeSize_ *
                                        rows / 2 + i * 1.5f * cubeSize_, -4.0f + 1.5f * j, - 12.5f + -j * 1.5f * cubeSize_}));

            cubes_[i * rows + j]->setColor(color_);
            cubes_[i * rows + j]->render(m_program_);
        }
    }

    update();

    ++frame_;
    ++frameCount_;

    if (time_.msecsTo(QTime::currentTime()) > 1000) {
        emit updateFPS("FPS: " + QString::number(frameCount_ / time_.secsTo(QTime::currentTime())));
        time_ = QTime::currentTime();
        frameCount_ = 0;
    }
}


QMatrix4x4 CubeWidget::setMVPMatrix(const QVector3D& pos){
    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 1.8f, 0.1f, 100.0f);
    matrix.translate(pos.x(), pos.y(), pos.z());
    matrix.rotate(100.0f * frame_ / 60.0f, 0, 1, 0);
    return matrix;
}
