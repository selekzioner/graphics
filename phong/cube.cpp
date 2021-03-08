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
}


void Cube::render(QOpenGLShaderProgram& program)
{
    std::for_each(surfaces_.begin(), surfaces_.end(), [&](Square& square){
        square.render(program);
    });
}


void Cube::setColor(const QColor& color)
{
    std::for_each(surfaces_.begin(), surfaces_.end(), [&](Square& square){
        square.setColor(color);
    });
}
