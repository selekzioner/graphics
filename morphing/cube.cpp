#include "cube.h"

Cube::Cube(GLfloat edgeLen, unsigned stepCount)
{
    for (auto i = 0; i < 6; ++ i) {
        surfaces_.emplace_back(edgeLen, stepCount);
    }
}


void Cube::initialize()
{
    initializeOpenGLFunctions();
    for (auto& surface : surfaces_) {
        surface.initialize();
    }

    QMatrix4x4 matrix;
    matrix.rotate(90.f, 0.f, 1.f);
    surfaces_[1].transform(matrix);

    matrix.rotate(90.f, 0.f, 1.f);
    surfaces_[2].transform(matrix);

    matrix.rotate(90.f, 0.f, 1.f);
    surfaces_[3].transform(matrix);

    matrix.rotate(90.f, 0.f, 1.f);

    matrix.rotate(90.f, 1.f, 0.f);
    surfaces_[4].transform(matrix);

    matrix.rotate(180.f, 1.f, 0.f);
    surfaces_[5].transform(matrix);

    const QVector3D scale = { 2.0, 2.0, 2.0 };
    QMatrix4x4 scale_mat;
    scale_mat.scale(scale);

    for (auto& surface : surfaces_){
        surface.transform(scale_mat);
    }
}


void Cube::render(QOpenGLShaderProgram& program)
{
    for (auto& surface : surfaces_) {
        surface.render(program);
    }
}


void Cube::setColor(const QColor& color)
{
    for (auto& surface : surfaces_) {
        surface.setColor(color);
    }
}
