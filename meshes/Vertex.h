#pragma once
#include <vector>
#include <QVector3D>
#include <QVector2D>

struct Vertex {
	QVector3D pos;
	QVector3D normal;
	QVector2D texCoord;
};

using Vertices = std::vector<Vertex>;
using Indices = std::vector<unsigned>;

enum class GLMode { TRIANGLES = 0, QUADS = 1 };