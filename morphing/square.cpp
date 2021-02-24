#include "square.h"


Square::Square(const GLfloat edgeLen, const unsigned stepCount) : edgeLen_(edgeLen),
    stepCount_(stepCount), vertexBuffer_(QOpenGLBuffer::VertexBuffer), indexBuffer_(QOpenGLBuffer::IndexBuffer) {}


void Square::initialize()
{
    initializeOpenGLFunctions();
    vertexBuffer_.create(); indexBuffer_.create();

    setVertices();
    setIndices();

    vertexBuffer_.bind();
    vertexBuffer_.allocate(vertices_.data(), vertices_.size() * sizeof(QVector3D));

    indexBuffer_.bind();
    indexBuffer_.allocate(indices_.data(), indices_.size() * sizeof(unsigned));
}


void Square::render(QOpenGLShaderProgram& program)
{
    vertexBuffer_.bind();
    indexBuffer_.bind();

    auto posAttr = program.attributeLocation("posAttr");
    program.enableAttributeArray(posAttr);
    program.setAttributeBuffer(posAttr, GL_FLOAT, 0, 3);

    program.setUniformValue("col", color_);

    glPolygonMode(GL_FRONT, GL_FILL);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, nullptr);

    QColor lines_col  {0, 0, 0};
    program.setUniformValue("col", lines_col);

    glPolygonMode(GL_FRONT, GL_LINE);
    glLineWidth(4.0f);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, nullptr);
}


void Square::setColor(const QColor& color)
{
    color_ = color;
}


void Square::transform(const QMatrix4x4& matrix)
{
    for (auto& v : vertices_){
        v = matrix * v;
    }
    vertexBuffer_.bind();
    vertexBuffer_.allocate(vertices_.data(), vertices_.size() * sizeof(QVector3D));
}


void Square::setVertices()
{
    auto step = edgeLen_ / stepCount_;
    auto a = edgeLen_ / 2;

    for (unsigned j = 0; j <= stepCount_; ++j) {
        for (unsigned i = 0; i <= stepCount_; ++i) {
            vertices_.push_back({-a + step * i, -a + step * j, a});
        }
    }
}


void Square::setIndices()
{
    for (unsigned i = 0; i < stepCount_; ++i) {
        for (unsigned j = 0; j < stepCount_; ++j) {

            auto first = i + (j + 1) * (stepCount_ + 1);
            auto second = i + j * (stepCount_ + 1);

            indices_.push_back(first); indices_.push_back(second); indices_.push_back(second + 1);

            indices_.push_back(second + 1); indices_.push_back(first + 1); indices_.push_back(first);
        }
    }
}


Square::~Square()
{
    vertexBuffer_.destroy();
    indexBuffer_.destroy();
}
