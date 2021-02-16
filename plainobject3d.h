#ifndef OBJECT3D_H
#define OBJECT3D_H
#include <QOpenGLFunctions>
#include <QColor>


class PlainObject3D
{
public:
    PlainObject3D() = default;

    void initialize();
    void render(GLint* posAttr, GLint* colAttr);

    virtual void setColor(const QColor& color) = 0;

    GLfloat const* getVertices() const;
    GLfloat const* getColors() const;

protected:
    std::vector<GLfloat> vertices_;
    std::vector<GLfloat> colors_;

    GLuint verticesCount;

    virtual void setVertices() = 0;
};

#endif // OBJECT3D_H
