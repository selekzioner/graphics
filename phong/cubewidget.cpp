#include "cubewidget.h"
#include <QMouseEvent>


CubeWidget::CubeWidget(QWidget* parent) : QOpenGLWidget(parent) {}


void CubeWidget::setCubesColor(const QColor& color)
{
    std::for_each(cubes_.begin(), cubes_.end(), [&](std::unique_ptr<Cube>& cube){
        cube->setColor(color);
    });
}


void CubeWidget::setNumberOfCubes(int value)
{
    numberOfCubes_ = value != 1 ? 2 * value : 1;
    initCubes();
}


void CubeWidget::setAmbientColor(const QColor& color)
{
    ambientColor_ = color;
}


void CubeWidget::setPointLightColor(const QColor& color)
{
    pointLight_.setColor(color);
}


void CubeWidget::setDirectedLightColor(const QColor& color)
{
    directedLight_.setColor(color);
}


void CubeWidget::setProjectorColor(const QColor& color)
{
    projectorLight_.setColor(color);
}


void CubeWidget::setPointLightX(double x)
{
    pointLight_.setPos( { static_cast<float>(x), pointLight_.getPos().y(), pointLight_.getPos().z() } );
}


void CubeWidget::setPointLightY(double y)
{
    pointLight_.setPos( { pointLight_.getPos().x(), static_cast<float>(y), pointLight_.getPos().z() } );
}


void CubeWidget::setPointLightZ(double z)
{
    pointLight_.setPos( { pointLight_.getPos().x(), pointLight_.getPos().y(), static_cast<float>(z) } );
}


void CubeWidget::setDirectedLightX(double x)
{
    directedLight_.setPos( { static_cast<float>(x), directedLight_.getPos().y(), directedLight_.getPos().z() } );
}


void CubeWidget::setDirectedLightY(double y)
{
    directedLight_.setPos( { directedLight_.getPos().x(), static_cast<float>(y), directedLight_.getPos().z() } );
}


void CubeWidget::setDirectedLightZ(double z)
{
    directedLight_.setPos( { directedLight_.getPos().x(), directedLight_.getPos().y(), static_cast<float>(z) } );
}


void CubeWidget::setProjectorLightX(double x)
{
    projectorLight_.setPos( { static_cast<float>(x), projectorLight_.getPos().y(), projectorLight_.getPos().z() } );
}


void CubeWidget::setProjectorLightY(double y)
{
    projectorLight_.setPos( { projectorLight_.getPos().x(), static_cast<float>(y), projectorLight_.getPos().z() } );
}


void CubeWidget::setProjectorLightZ(double z)
{
    projectorLight_.setPos( { projectorLight_.getPos().x(), projectorLight_.getPos().y(), static_cast<float>(z) } );
}


void CubeWidget::setAmbientCoef(int value)
{
    ka_ = static_cast<float>(value) / 20;
}


void CubeWidget::setDiffuseCoef(int value)
{
    kd_ = static_cast<float>(value) / 20;
}


void CubeWidget::setSpecularCoef(int value)
{
    ks_ = static_cast<float>(value) / 20;
}


void CubeWidget::switchAmbient()
{
    isAmbientEnabled_ = !isAmbientEnabled_;
}


void CubeWidget::switchPointLight()
{
    pointLight_.isEnabled = !pointLight_.isEnabled;
}


void CubeWidget::switchDirectedLight()
{
    directedLight_.isEnabled = !directedLight_.isEnabled;
}


void CubeWidget::switchProjector()
{
    projectorLight_.isEnabled = !projectorLight_.isEnabled;
}


void CubeWidget::initializeGL()
{
    directedLight_.setPos({ -30.f, 10.f, -10.f });
    directedLight_.initialize();

    pointLight_.setPos({ 30.f, 10.f, -10.f });
    pointLight_.setColor( { 255, 255, 0 } );
    pointLight_.initialize();

    projectorLight_.setPos( { 0.f, 10.f, -30.f } );
    projectorLight_.setColor( { 0, 0, 255 } );
    projectorLight_.initialize();

    initializeOpenGLFunctions();
    initShaders();
    initCubes();

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    time_ = QTime::currentTime();
}


void CubeWidget::initShaders()
{
    objectShader_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/objects.vsh");
    objectShader_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/objects.fsh");
    objectShader_.link();

    lighterShader_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/lighter.vsh");
    lighterShader_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/lighter.fsh");
    lighterShader_.link();
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
    objectShader_.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    objectShader_.setUniformValue("ka", ka_);
    objectShader_.setUniformValue("kd", kd_);
    objectShader_.setUniformValue("ks", ks_);

    objectShader_.setUniformValue("ambientColor", ambientColor_);
    objectShader_.setUniformValue("isAmbient", isAmbientEnabled_);

    objectShader_.setUniformValue("pointColor", pointLight_.getColor());
    objectShader_.setUniformValue("pointPos", pointLight_.getPos());
    objectShader_.setUniformValue("isPoint", pointLight_.isEnabled);

    objectShader_.setUniformValue("directedColor", directedLight_.getColor());
    objectShader_.setUniformValue("directedPos", directedLight_.getPos());
    objectShader_.setUniformValue("isDirected", directedLight_.isEnabled);

    objectShader_.setUniformValue("projectorColor", projectorLight_.getColor());
    objectShader_.setUniformValue("projectorPos", projectorLight_.getPos());
    objectShader_.setUniformValue("isProjector", projectorLight_.isEnabled);

    objectShader_.setUniformValue("view", view_.getViewMatrix());
    objectShader_.setUniformValue("viewPos", view_.getPos());

    QMatrix4x4 projection;
    projection.perspective(80.0f, 1.3f, 0.1f, 1000000.0f);
    objectShader_.setUniformValue("projection", projection);

    const auto rows = static_cast<int>(cubes_.size() / std::sqrt(cubes_.size()));
    const auto cols = static_cast<int>(cubes_.size() / rows);

    for (auto i = 0; i < cols; ++i){
        for (auto j = 0; j < rows; ++j){
            QMatrix4x4 model;
            model.translate(- 1.5f * cubeSize_ * rows / 2 + i * 2.f * cubeSize_,
                                /*-4.0f + 1.5f * j*/0, - 12.5f + -j * 2.f * cubeSize_);
            model.rotate(80.0f * frame_ / 60.0f, 0, 1, 0);
            objectShader_.setUniformValue("model", model);
            cubes_[i * rows + j]->render(objectShader_);
        }
    }

    lighterShader_.bind();
    if (pointLight_.isEnabled) {
        QMatrix4x4 pointMatrix;
        pointMatrix.translate(pointLight_.getPos());
        lighterShader_.setUniformValue("matrix", projection * view_.getViewMatrix() * pointMatrix);
        pointLight_.render(lighterShader_);
    }

    if (directedLight_.isEnabled) {
        QMatrix4x4 directedMatrix;
        directedMatrix.translate(directedLight_.getPos());
        lighterShader_.setUniformValue("matrix", projection * view_.getViewMatrix() * directedMatrix);
        directedLight_.render(lighterShader_);
    }

    if (projectorLight_.isEnabled) {
        QMatrix4x4 projectorMatrix;
        projectorMatrix.translate(projectorLight_.getPos());
        lighterShader_.setUniformValue("matrix", projection * view_.getViewMatrix() * projectorMatrix);
        projectorLight_.render(lighterShader_);
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
