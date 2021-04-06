#pragma once

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_3_0>
#include <QOpenGLTexture>

#include "Vertex.h"

struct Material {
	QColor diffuse, specular;
};

class Mesh;
using MeshPtr = std::unique_ptr<Mesh>;

class Mesh final : protected QOpenGLFunctions_3_0 {
public:
	Mesh();
	~Mesh();

	void Initialize();
	void Render(QOpenGLShaderProgram& program);

	Vertices vertices;
	Indices indices;

	std::shared_ptr<QOpenGLTexture> texture, normalMap;

	GLMode mode = GLMode::TRIANGLES;

	Material material = { { 255, 255, 175 },
											 { 255, 255, 255} };

private:
	QOpenGLBuffer _vertexBuf, _indexBuf;
};
