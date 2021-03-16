#include "cube.h"

Cube::Cube(GLfloat edgeLen, unsigned stepCount) : edgeLen_(edgeLen),
    stepCount_(stepCount), vertexBuffer_(QOpenGLBuffer::VertexBuffer), indexBuffer_(QOpenGLBuffer::IndexBuffer) {}


void Cube::initialize()
{
    initializeOpenGLFunctions();
    vertexBuffer_.create(); indexBuffer_.create();

    QMatrix4x4 matrix;
    setVertices(matrix);

    matrix.rotate(90.f, 0.f, 1.f);
    setVertices(matrix);

    matrix.rotate(90.f, 0.f, 1.f);
    setVertices(matrix);

    matrix.rotate(90.f, 0.f, 1.f);
    setVertices(matrix);

    matrix.rotate(90.f, 0.f, 1.f);
    matrix.rotate(90.f, 1.f, 0.f);
    setVertices(matrix);

    matrix.rotate(180.f, 1.f, 0.f);
    setVertices(matrix);

    vertexBuffer_.bind();
    vertexBuffer_.allocate(vertices_.data(), vertices_.size() * sizeof(Vertex));

    indexBuffer_.bind();
    indexBuffer_.allocate(indices_.data(), indices_.size() * sizeof(unsigned));
}


void Cube::render(QOpenGLShaderProgram& program)
{
    program.enableAttributeArray("posAttr");
    program.setAttributeBuffer("posAttr", GL_FLOAT, 0, 3, sizeof(Vertex));

    program.enableAttributeArray("normAttr");
    program.setAttributeBuffer("normAttr", GL_FLOAT, offsetof(Vertex, normal), 3, sizeof(Vertex));

    program.setUniformValue("objectColor", color_);

    glPolygonMode(GL_FRONT, GL_FILL);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, nullptr);
}


void Cube::setVertices(const QMatrix4x4& matrix)
{
    auto step = edgeLen_ / stepCount_;
    auto a = edgeLen_ / 2;
    auto prevSize = vertices_.size();

    for (unsigned j = 0; j <= stepCount_; ++j) {
        for (unsigned i = 0; i <= stepCount_; ++i) {
            vertices_.push_back({{-a + step * i, -a + step * j, a},
                                 {0.f, 0.f, 1.f}});
        }
    }

    std::for_each (vertices_.begin() + prevSize, vertices_.end(), [&](Vertex& v){
        v = { matrix * v.vertex, matrix.inverted().transposed() * v.normal };
    });

    for (unsigned i = 0; i < stepCount_; ++i) {
        for (unsigned j = 0; j < stepCount_; ++j) {

            const auto first = prevSize + i + (j + 1) * (stepCount_ + 1);
            const auto second = prevSize + i + j * (stepCount_ + 1);

            indices_.push_back(first); indices_.push_back(second); indices_.push_back(second + 1);
            indices_.push_back(second + 1); indices_.push_back(first + 1); indices_.push_back(first);
        }
    }
}


Cube::~Cube()
{
    vertexBuffer_.destroy();
    indexBuffer_.destroy();
}
